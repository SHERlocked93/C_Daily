//
// Created by qiany on 2024/6/14.
//
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mq_mtx;
std::condition_variable cv;
bool ready = false;
int shared_data = 0;

void producer() {
  {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::lock_guard<std::mutex> lock(mq_mtx);
    shared_data = 42;  // 修改共享数据
    ready = true;      // 设置条件为真
  }
  cv.notify_one();  // 通知一个等待线程
}

void consumer() {
  std::unique_lock<std::mutex> lock(mq_mtx);
  cv.wait(lock, [] { return ready; });  // 等待直到ready为真
  std::cout << "Shared data: " << shared_data << std::endl;
}

int main() {
  std::cout << "hello!" << std::endl;

  std::thread t1(producer);
  std::thread t2(consumer);

  t1.join();
  t2.join();

  return 0;
}
