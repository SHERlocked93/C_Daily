#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

int main() {
  auto vec = std::views::iota(0, 11);
  std::ranges::for_each(vec, [](auto item) { std::cout << item << ","; });
  std::cout << std::endl;

  auto r = std::ranges::reverse_view(vec);
  std::ranges::for_each(r, [](auto item) { std::cout << item << ","; });
  std::cout << std::endl;

  for (const auto item : std::ranges::filter_view(r, [](auto it) { return it % 2; })) std::cout << item << ",";
  std::cout << std::endl;

  for (const auto item : std::views::iota(0, 20) | std::views::reverse | std::views::filter([](auto it) { return it % 2; })) std::cout << item << ",";
  std::cout << std::endl;

  auto vec2 = std::views::iota(0, 101);
  const auto rdc = std::accumulate(vec2.begin(), vec2.end(), 0, [](auto acc, auto it) { return acc + it * it; });
  std::cout << "acc:" << rdc << std::endl;

  return 0;
}
