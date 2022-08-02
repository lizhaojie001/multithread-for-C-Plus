# Multithread-for-C-Plus 

`互斥量模板类`
>C++标准库为互斥量提供了RAII模板类 std::lock_guard ，在构造时就能提供已锁的互斥量，并在析构时进行解
锁，从而保证了互斥量能被正确解锁



[01 std::once() 多线程操作,保证资源初始化执行一次](./01_std_call_once/main.cpp)

除了使用这种方式也可以采用静态变量的方式初始化
```c++
class my_class;
myclass& get_my_class_instance() {
 static my_class instance; //线程安全
 return instance;
}
```
