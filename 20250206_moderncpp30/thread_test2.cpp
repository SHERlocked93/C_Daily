//
// Created by Ǯ�� on 2025/2/9.
//

#include <chrono>
#include <future>
#include <iostream>
#include <thread>
using namespace std;

int work() {
  //  ��װ���Ǽ����˺ܾ�
  this_thread::sleep_for(2s);
  return 42;
}


int main(int argc) {
  char* ptr = new char[20];
  auto fut = async(launch::async, work);

  //  ��һЩ������
  cout << "I am waiting now\n";
  cout << "Answer: " << fut.get() << '\n';
}
