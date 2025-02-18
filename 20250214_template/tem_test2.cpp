//
// Created by qiany on 2025/2/14.
//
#include <iostream>

template <typename T1, typename T2>
auto add(T1 a, T2 b) {
  auto temp = a + b;
  return temp;
}

int main() {
  std::cout << add(1.1, 2) << std::endl;
  return 0;
}
