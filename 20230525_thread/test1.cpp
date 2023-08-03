#include <cstdio>
#include <future>
#include <iostream>
#include <thread>

using namespace std;

int g_count = 0;
// atomic<int> g_count{0};  // 可以用atomic解决这个情况

/// \brief 线程函数
/// \param n 循环次数
void thread_func(int n)
{
    for (int i = 0; i < n; ++i) {
        g_count++;
        // g_count = g_count + 1; // 原子操作不支持这种写法
    }
}

int main()
{
    future_status status;
    thread t1(thread_func, 1000000);
    thread t2(thread_func, 1000000);
    
    t1.join();
    t2.join();
    cout << "g_count:" << g_count << endl;
}
