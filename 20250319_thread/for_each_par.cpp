//
// Created by qiany on 2025/3/27.
//

#include <algorithm>
#include <execution>
#include <iostream>
#include <thread>
#include <vector>

int main() {
  // ��ʼ��һ������ 1 �� 10 ������
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  std::cout << " main thread " << std::this_thread::get_id() << std::endl;

  // ʹ�� std::for_each ����ִ�У���ӡÿ��Ԫ�غ��߳� ID
  std::for_each(std::execution::par, numbers.begin(), numbers.end(),
                [](int n) { std::cout << "Processing " << n << " in thread " << std::this_thread::get_id() << std::endl; });

  return 0;
}
