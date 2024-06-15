//
// Created by qiany on 2024/6/14.
//
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <thread>

auto TIMEOUT = std::chrono::seconds(3);
std::condition_variable cv;
std::mutex cv_m;  // 此互斥体用于三个目的：
                  // 1) 同步对 i 的访问
                  // 2) 同步对 std::cerr 的访问
                  // 3) 用于条件变量 cv
int i = 0;

void waits(int id, std::chrono::seconds timeout) {
  std::unique_lock<std::mutex> lk(cv_m);
  std::cerr << "等待... \n";
  bool isTimeout = !cv.wait_for(lk, TIMEOUT, [id] { return i == id; });
  std::cerr << "...结束等待。i == " << id << " - timeout:" << isTimeout << std::endl;
}

void signals() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  {
    std::lock_guard<std::mutex> lk(cv_m);
    std::cerr << "通知...\n";
  }
  cv.notify_all();

  std::this_thread::sleep_for(std::chrono::seconds(1));

  {
    std::lock_guard<std::mutex> lk(cv_m);
    i = 2;
    std::cerr << "再次通知...\n";
  }
  cv.notify_all();
}

int main() {
  std::cout << "hello!" << std::endl;
  std::thread t1(waits, 1, TIMEOUT), t2(waits, 2, TIMEOUT), t4(signals), t3(waits, 3, TIMEOUT);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
}
