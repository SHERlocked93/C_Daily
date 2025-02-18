#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

int main() {
  // auto res = std::views::iota(1) | std::views::transform([](const int i) { return i * i; }) | std::views::filter([](const int i) { return i % 2;
  // });
  //
  // // std::vector<int> vec;
  // // vec = res | std::ranges::to<std::vector<int>>(); // c23
  // // std::ranges::copy(res, std::back_inserter(vec));
  //
  // const auto sum = std::accumulate(std::begin(res), std::end(res), 0);

  auto res = std::views::iota(1, 10) | std::views::filter([](const int n) { return n % 2 == 0; }) |
             std::views::transform([](int n) { return n * 3; }) | std::views::take(5) | std::views::common;

  int sum = std::accumulate(std::begin(res), std::end(res), 0);


  std::cout << "sum:" << sum << std::endl;

  return 0;
}
