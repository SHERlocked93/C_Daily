//
// Created by qiany on 2025/3/27.
//

#include <algorithm>
#include <execution>
#include <iostream>
#include <thread>
#include <vector>

int main() {
  // 初始化一个包含 1 到 10 的向量
  std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  std::cout << " main thread " << std::this_thread::get_id() << std::endl;

  // 使用 std::for_each 并行执行，打印每个元素和线程 ID
  std::for_each(std::execution::par, numbers.begin(), numbers.end(),
                [](int n) { std::cout << "Processing " << n << " in thread " << std::this_thread::get_id() << std::endl; });

  return 0;
}
