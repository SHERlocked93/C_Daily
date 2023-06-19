//
// Created by 钱昱 on 2023/2/27.  C++ primer P362
//
#include <algorithm>
#include <bitset>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <numeric>
#include <regex>
#include <vector>

using namespace std;

// 将整数类型数值转换为二进制，并每8位添加一个空格
template <typename T> void printBinaryWithSpaces(T value) {
  constexpr int width = std::numeric_limits<T>::digits; // 获取T类型的最大位数
  bitset<width> binary(value);

  int count = 0;                         // 记录已输出位数
  for (int i = width - 1; i >= 0; --i) { // 倒序输出二进制数
    std::cout << binary[i];
    if (++count % 8 == 0 &&
        count != width) { // 每8位添加一个空格，注意末尾不需要空格
      std::cout << " ";
    }
  }
}

int main() {
  int intValue = 1024;
  printBinaryWithSpaces(intValue);

  return 0;
}
