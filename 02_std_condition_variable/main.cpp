#include <QCoreApplication>
#include <queue>
#include <QDebug>
#include <thread>
class data_chunk
{
public:
    data_chunk() {}
    void do_something(){
        qDebug() << "消费数据";
    };
};


std::mutex mut;
std::condition_variable data_con;
std::queue<data_chunk*> data_queue;
bool stop = false;

void produce_data_thread() {
    while (1) {
        data_chunk *data = new data_chunk;
        std::lock_guard<std::mutex> gk(mut);
        data_queue.push(data);
        data_con.notify_one ();
        qDebug() << "生产数据";
        if (data_queue.size () == 200) {
            stop = true;
            break;
        }
    }
}


void consume_data_thread() {
    while (1) {
        std::unique_lock<std::mutex> lk(mut);
        data_con.wait (lk,[]{return !data_queue.empty ();});
        data_chunk * data = data_queue.front ();
        data_queue.pop ();
        lk.unlock ();
        data->do_something ();
        if (data_queue.empty () && stop) {
            break;
        }
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::thread([]{
        produce_data_thread ();
    }).detach();

    std::thread([]{
        consume_data_thread ();
    }).detach ();

    return a.exec();
}
