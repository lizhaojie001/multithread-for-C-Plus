#include <QCoreApplication>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <thread>
#include <QDebug>

class data_chunk
{
public:
    data_chunk() {}
    void do_something(){
//        qDebug() << "消费数据";
    }
};

template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue()
    {}
    threadsafe_queue(threadsafe_queue const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue=other.data_queue;
    }

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty)
            return false;
        value=data_queue.front();
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    threadsafe_queue<data_chunk*> queue;
    bool stop = false;
    int i = 200;
    int j = 0;
    std::thread([&]{

        while ( i >0) {
            queue.push (new data_chunk);
            i--;
            qDebug() << "生产数据" << i;
        }
        stop = true;
    }).detach ();

    std::thread([&]{
        while (1) {
            data_chunk *data;
            queue.wait_and_pop (data);
            data->do_something ();
            qDebug() << "消费数据" << j++;
            if (queue.empty () && stop) {
                break;
            }
        }
    }).detach ();

    return a.exec();
}
