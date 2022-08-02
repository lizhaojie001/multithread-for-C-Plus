# Multithread-for-C-Plus 

[01 std::once() 多线程操作,保证资源初始化执行一次](./01_std_call_once/main.cpp)

除了使用这种方式也可以采用静态变量的方式初始化
```c++
class my_class;
myclass& get_my_class_instance() {
 static my_class instance; //线程安全
 return instance;
}
```
