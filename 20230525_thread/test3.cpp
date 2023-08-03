#include <future>
#include <iostream>

using namespace std;

void async_operation(promise<int>& p)
{
    // 模拟异步操作
    this_thread::sleep_for(3s);
    p.set_value(33); // 将结果设置到 promise 对象中
}

int main()
{
    promise<int> myPromise;
    future<int> myFuture = myPromise.get_future();
    cout << "in main" << endl;

    // 在另一个线程中执行异步操作
    thread t(async_operation, ref(myPromise));

    // 在主线程中获取异步操作的结果
    int result = myFuture.get();

    cout << "Result: " << result << endl;

    t.join();
    return 0;
}
