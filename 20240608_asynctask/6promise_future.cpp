#include <iostream>
#include <string>

#include <future>
#include <thread>
#include <chrono>

// 模拟一个异步操作
void asyncOperation(std::promise<int> promise) {
  std::this_thread::sleep_for(std::chrono::seconds(2));  // 模拟耗时操作
  int result = 43;                                       // 假设这是计算得到的结果
  promise.set_value(result);                             // 设置结果
}

int main() {
  // 创建 promise 和 future
  std::promise<int> resultPromise;
  std::future<int> resultFuture = resultPromise.get_future();

  // 启动异步操作
  std::thread asyncThread(asyncOperation, std::move(resultPromise));

  // 等待结果
  std::cout << "Waiting for the result..." << std::endl;
  int result = resultFuture.get();  // 这会阻塞，直到结果可用
  std::cout << "Result: " << result << std::endl;

  // 等待线程完成
  asyncThread.join();

  return 0;
}
