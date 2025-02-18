//
// Created by qiany on 2025/2/14.
//
#include <iostream>

template <size_t N>
struct Fibonacci {
  constexpr static size_t value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template <>
struct Fibonacci<0> {
  constexpr static size_t value = 0;
};
template <>
struct Fibonacci<1> {
  constexpr static size_t value = 1;
};

int main() {
  static_assert(Fibonacci<10>::value == 55, "Fibonacci<10>::value == 55");
  return 0;
}
