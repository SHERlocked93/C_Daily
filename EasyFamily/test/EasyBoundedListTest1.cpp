#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "../EasyBoundedList.h"

using namespace common_util;

void test_basic_operations() {
  std::cout << "Testing basic operations..." << std::endl;

  BoundedQueue<int> queue(2);

  assert(queue.empty());
  assert(queue.size() == 0);

  queue.push(std::make_unique<int>(1));
  assert(!queue.empty());
  assert(queue.size() == 1);

  queue.push(std::make_unique<int>(2));
  assert(queue.size() == 2);

  auto val1 = queue.pop();
  assert(*val1 == 1);
  assert(queue.size() == 1);

  auto val2 = queue.pop();
  assert(*val2 == 2);
  assert(queue.empty());

  std::cout << "  Basic operations passed." << std::endl;
}

void test_concurrency() {
  std::cout << "Testing concurrency..." << std::endl;

  BoundedQueue<int> queue(5);
  const int num_items = 100;

  std::thread producer([&]() {
    for (int i = 0; i < num_items; ++i) {
      queue.push(std::make_unique<int>(i));
      // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  });

  std::thread consumer([&]() {
    for (int i = 0; i < num_items; ++i) {
      auto val = queue.pop();
      assert(*val == i);
    }
  });

  producer.join();
  consumer.join();

  assert(queue.empty());
  std::cout << "  Concurrency passed." << std::endl;
}

int main() {
  test_basic_operations();
  test_concurrency();

  std::cout << "All EasyBoundedList tests passed!" << std::endl;
  return 0;
}
