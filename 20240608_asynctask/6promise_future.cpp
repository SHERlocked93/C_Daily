#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using std::chrono_literals;

int main() {
  // 创建 promise 和 future
  std::promise<int> resultPromise;
  std::future<int> resultFuture = resultPromise.get_future();

  // 启动异步操作
  std::thread asyncThread(
      [](auto promise) {
        std::this_thread::sleep_for(std::chrono::seconds(2));  // 模拟耗时操作

        promise.set_value(43);  // 设置结果
      },
      std::move(resultPromise));

  // 等待结果
  std::cout << "Waiting for the result..." << std::endl;
  int result = resultFuture.wait_for(2000ms);  // 这会阻塞，直到结果可用
  std::cout << "Result: " << result << std::endl;

  // 等待线程完成
  asyncThread.join();

  return 0;
}
