#include <QCoreApplication>
#include <QDebug>
#include <thread>
#include <mutex>
class Resource
{
public:
    Resource() {}
    int a = 0;
    void do_something() {
        qDebug()<< this->a;
    }
};
std::shared_ptr<Resource> resource_ptr;
std::once_flag resource_flag;

void init_resource() {
    resource_ptr.reset(new Resource);
    resource_ptr->a = resource_ptr->a + 1;
}

void foo() {
    //可以完成的进行一次初始化
    std::call_once(resource_flag,init_resource);
    resource_ptr->do_something ();
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    for (int i = 0; i < 10; ++i) {
        std::thread([=]() {
            foo();
        }).detach();
    }

/*打印结果全为1,可知只执行了一次 初始化*/

    return a.exec();
}
