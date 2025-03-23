//
// Created by qiany on 2025/3/19.
//
#include <chrono>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

using namespace std::chrono_literals;

int hellocount = 1;

int main() {
  std::shared_mutex smtx_;  // 共享锁

  const auto threadWrite = [&]() {
    while (true) {
      smtx_.lock_shared();
      std::this_thread::sleep_for(20ms);
      std::cout << "[" << std::this_thread::get_id() << "] <- get count: " << hellocount << std::endl;
      smtx_.unlock_shared();

      smtx_.lock();
      std::this_thread::sleep_for(200ms);
      hellocount++;  // write
      std::cout << "[" << std::this_thread::get_id() << "] -> write count: " << hellocount << std::endl;
      smtx_.unlock();
    }
  };

  const auto threadRead = [&]() {
    while (true) {
      smtx_.lock_shared();
      std::this_thread::sleep_for(20ms);
      std::cout << "[" << std::this_thread::get_id() << "] <- get count: " << hellocount << std::endl;
      smtx_.unlock_shared();
    }
  };

  // 启三个读三个写线程
  std::vector<std::thread> threads;
  for (int i = 0; i < 3; ++i) threads.emplace_back(threadRead);
  for (int i = 0; i < 3; ++i) threads.emplace_back(threadWrite);

  std::cout << "Hello, World!" << std::endl;

  std::cin.get();
  for (auto& t : threads) t.join();

  return 0;
}
