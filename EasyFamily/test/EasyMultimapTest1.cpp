#include <iostream>

#include "../EasyMultimap.h"

int main() {
  using namespace common_util;
  try {
    std::cout << "=== Testing addTuple (unique values) ===" << std::endl;
    // Initialize with 4-element tuple: int, string, int, double
    EasyMultimap<int, std::string, int, double> em1{
        {1, "hello", 11, 543.1},
        {4, "world", 88, 545.0},
    };
    // Using uniqueness check: no duplicate allowed
    em1.insert({2, "yyy", 99, 1.2});
    em1.insert({3, "yyy", 99, 1.2});
    // Uncommenting the following line would throw an exception due to duplicate in a column:
    em1.insert({4, "zzz", 100, 2.3});

    // std::cout << "=== Testing addTuple (allows duplicates) ===" << std::endl;
    // // Using non-check method: duplicates are allowed
    // em1.addTupleWithoutCheck({4, "zzz", 100, 2.3});
    // em1.addTupleWithoutCheck({4, "abc", 101, 9.9});

    std::cout << "=== Testing getValueByAnotherColumn ===" << std::endl;
    // Find the first tuple whose 0-th column equals 4, and return its 3rd column value
    auto res = em1.getVal<0, 3>(4);
    std::cout << "Found: " << res << std::endl;  // Expected output: 545.0 (from the first matching tuple)
    auto res1 = em1.getVal<0, 1>(2);
    std::cout << "Found: " << res1 << std::endl;

    auto column1 = em1.getColumn<1>();
    std::cout << "Column 1 (strings): ";
    for (const auto& val : column1) {
      std::cout << val << " ";
    }
    std::cout << std::endl;  // export: hello world yyy

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
