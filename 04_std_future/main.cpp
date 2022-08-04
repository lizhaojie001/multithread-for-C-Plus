#include <QCoreApplication>
#include <future>
#include <QDebug>
#include <thread>
#include <QThread>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    std::launch::async  开启新线程
//    std::launch::defered  等待调用get 或者wait执行 ,不调用不执行

    auto addc =  std::async( std::launch::deferred,[]{
        std::this_thread::sleep_for (std::chrono::microseconds(10000));
        qDebug() << "执行1" <<  QThread::currentThreadId ();
    });
    qDebug() << "执行2";
//    addc.get (); //阻塞线程
//    addc.wait ();
    qDebug() << "执行3"<<QThread::currentThreadId ();
    return a.exec();
}
