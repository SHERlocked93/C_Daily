#include "../EasyFormat.h"

int main() {
  int intValue = 886023;
  printBinaryWithSpaces(intValue);
  // 00000110 10111100 10001111

  long longValue = 123456789012345;
  printBinaryWithSpaces(longValue);
  // 00000000 00000000 00000000 00000000 00111010 01100011 01010100 11001101

  int intValue2 = 1024;
  printBinaryWithSpaces(intValue2);
  // 00000000 00000000 00001000 0000000

  std::cout << std::endl << std::endl;

  char arr[] = {1, 2, 3, (char)0xFE};
  printHexWithSpaces(arr, sizeof(arr));
  // 0x01 0x02 0x03 0xfe
}
