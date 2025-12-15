#include <cassert>
#include <iostream>
#include <string>

#include "../EasyStrConvert.h"

using namespace common_util;

void test_string_conversions() {
  std::cout << "Testing string conversions..." << std::endl;

  std::string s = "Hello World";
  std::wstring ws = to_wstring(s);
  std::string s2 = wto_string(ws);

  assert(s == s2);
  std::cout << "  to_wstring/wto_string passed." << std::endl;

  // Test w2c and c2w
  std::string s3 = w2c(ws.c_str());
  assert(s == s3);

  std::wstring ws2 = c2w(s.c_str());
  assert(ws == ws2);
  std::cout << "  w2c/c2w passed." << std::endl;
}

void test_hex_conversions() {
  std::cout << "Testing hex conversions..." << std::endl;

  // int2hexStr
  assert(int2hexStr(255, 2) == "ff");
  assert(int2hexStr(255, 4) == "00ff");

  // bytes2hexStr
  unsigned char bytes[] = {0x12, 0xAB, 0xCD};
  assert(bytes2hexStr(bytes, 3) == "12abcd");
  assert(bytes2hexStr(bytes, 3, true) == "12ABCD");
  assert(bytes2hexStr(bytes, 3, false, true) == "12 ab cd");

  // hexStr2Vector
  std::vector<unsigned char> vec = hexStr2Vector("12ABCD");
  assert(vec.size() == 3);
  assert(vec[0] == 0x12);
  assert(vec[1] == 0xAB);
  assert(vec[2] == 0xCD);

  std::cout << "  Hex conversions passed." << std::endl;
}

void test_other_utils() {
  std::cout << "Testing other utils..." << std::endl;

  // containsInVals
  assert(containsInVals(3, {1, 2, 3}));
  assert(!containsInVals(4, {1, 2, 3}));

  // str2vec / vec2str
  std::string csv = "1,2,3,4";
  std::vector<int> vec = str2vec<int>(csv);
  assert(vec.size() == 4);
  assert(vec[0] == 1);

  std::string csv2 = vec2str(vec);
  assert(csv == csv2);

  std::cout << "  Other utils passed." << std::endl;
}

int main() {
  test_string_conversions();
  test_hex_conversions();
  test_other_utils();

  std::cout << "All EasyStrConvert tests passed!" << std::endl;
  return 0;
}
