#include <cassert>
#include <iostream>
#include <string>

#include "../EasyEnum.h"

using namespace common_util;

enum class Color { RED, GREEN, BLUE };

void test_easy_enum() {
  std::cout << "Testing EasyEnum..." << std::endl;

  EasyEnum<Color, std::string> colorEnum({{Color::RED, "Red"}, {Color::GREEN, "Green"}, {Color::BLUE, "Blue"}});

  // Test get
  assert(colorEnum.get(Color::RED) == "Red");
  assert(colorEnum[Color::GREEN] == "Green");

  // Test getKeyOf
  assert(colorEnum.getKeyOf("Blue") == Color::BLUE);

  // Test hasKey / hasVal
  assert(colorEnum.hasKey(Color::RED));
  assert(!colorEnum.hasKey(static_cast<Color>(99)));
  assert(colorEnum.hasVal("Green"));
  assert(!colorEnum.hasVal("Yellow"));

  // Test add
  // Note: EasyEnum doesn't seem to have a public add method in the version I read?
  // Wait, I read the file and it DOES have `void add(const keyT& key, const valT& val)`.
  // Let's double check the file content I read earlier.
  // Yes: void add(const keyT& key, const valT& val) { myEnum_.emplace(key, val); }

  // However, emplace might not overwrite if key exists. Let's try adding a new one.
  // Actually I'll define a new enum value or just cast for test purposes if I can't extend the enum class easily.
  // But wait, I can just use int as key too.

  EasyEnum<int, std::string> intEnum;
  intEnum.add(1, "One");
  assert(intEnum.get(1) == "One");

  std::cout << "  EasyEnum basic tests passed." << std::endl;
}

void test_helper_functions() {
  std::cout << "Testing helper functions..." << std::endl;

  EasyEnum<int, std::string> numEnum({{1, "One"}, {2, "Two"}});

  assert(getValFromEnum(numEnum, 1) == "One");
  assert(getValFromEnum(numEnum, 3) == "");  // Default constructed string

  assert(getKeyFromEnum(numEnum, std::string("Two")) == 2);
  assert(getKeyFromEnum(numEnum, std::string("Three")) == 0);  // Default constructed int

  std::cout << "  Helper functions passed." << std::endl;
}

int main() {
  test_easy_enum();
  test_helper_functions();

  std::cout << "All EasyEnum tests passed!" << std::endl;
  return 0;
}
