//
// Created by qiany on 2024/6/14.
//
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <thread>

auto TIMEOUT = std::chrono::seconds(3);
std::condition_variable cv;
std::mutex cv_m;  // �˻�������������Ŀ�ģ�
                  // 1) ͬ���� i �ķ���
                  // 2) ͬ���� std::cerr �ķ���
                  // 3) ������������ cv
int i = 0;

void waits(int id, std::chrono::seconds timeout) {
  std::unique_lock<std::mutex> lk(cv_m);
  std::cerr << "�ȴ�... \n";
  bool isTimeout = !cv.wait_for(lk, TIMEOUT, [id] { return i == id; });
  std::cerr << "...�����ȴ���i == " << id << " - timeout:" << isTimeout << std::endl;
}

void signals() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  {
    std::lock_guard<std::mutex> lk(cv_m);
    std::cerr << "֪ͨ...\n";
  }
  cv.notify_all();

  std::this_thread::sleep_for(std::chrono::seconds(1));

  {
    std::lock_guard<std::mutex> lk(cv_m);
    i = 2;
    std::cerr << "�ٴ�֪ͨ...\n";
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
