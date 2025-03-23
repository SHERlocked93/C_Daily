#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

int main() {
  auto tp = std::make_tuple(1, 2, 3);

  auto [a, b, ignore] = tp;

  std::cout << a << " " << b << std::endl;

  return 0;
}
