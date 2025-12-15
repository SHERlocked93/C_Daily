#include <cassert>
#include <iostream>

#include "../EasyFinally.h"

using namespace common_util;

void test_basic_cleanup() {
  std::cout << "Testing basic cleanup..." << std::endl;
  bool cleaned_up = false;
  {
    EasyFinally cleanup([&]() {
      cleaned_up = true;
      std::cout << "  Cleanup executed." << std::endl;
    });
  }
  assert(cleaned_up);
  std::cout << "  Basic cleanup passed." << std::endl;
}

void test_dismiss() {
  std::cout << "Testing dismiss..." << std::endl;
  bool cleaned_up = false;
  {
    EasyFinally cleanup([&]() { cleaned_up = true; });
    cleanup.dismiss();
  }
  assert(!cleaned_up);
  std::cout << "  Dismiss passed." << std::endl;
}

int main() {
  test_basic_cleanup();
  test_dismiss();

  std::cout << "All EasyFinally tests passed!" << std::endl;
  return 0;
}
