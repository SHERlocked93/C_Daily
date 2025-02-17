//
// Created by qiany on 2024/11/26.
//
#include <algorithm>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

int main() {
  std::vector nums = {1, 2, 3, 4, 5};
  std::vector<int> res;

  // 使用函数对象对每个元素进行平方运算并存储到新容器中
  std::ranges::copy_if(nums, std::back_inserter(res), [](auto a) { return a % 2; });

  // 打印新容器的元素
  for (const auto& num : res) {
    std::cout << num << " ";
    }

    return 0;
}
