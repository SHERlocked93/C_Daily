#pragma once

#include <bitset>
#include <iomanip>
#include <iostream>
#include <limits>

//! 将整数类型数值转换为二进制，并每8位添加一个空格，输出为左高右低
template <typename T>
void printBinaryWithSpaces(T value) {
  constexpr int width = std::numeric_limits<T>::digits;  // 获取T类型的最大位数
  std::bitset<width> binary(value);

  int count = 0;                          // 记录已输出位数
  for (int i = width - 1; i >= 0; --i) {  // 倒序输出二进制数
    std::cout << binary[i];
    if (++count % 8 == 0 && count != width) {  // 每8位添加一个空格，注意末尾不需要空格
      std::cout << " ";
    }
  }
}

//! 打印数组的十六进制数据
template <typename T>
void printHexWithSpaces(const T* dataArr, size_t dataLen) {
  for (size_t i = 0; i < dataLen; ++i) {
    std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(dataArr[i]));
    if (i != dataLen - 1) {
      std::cout << " ";
    }
  }
  std::cout << std::dec;
}
