#include <iostream>
#include <string>

inline int bcd2num(const unsigned char* pos, int ptrLen) {
  int result = 0;
  for (int i = 0; i < ptrLen; i++) {
    result = result * 100 + ((pos[i] & 0x0F) + ((pos[i] >> 4) * 10));
  }

  return result;
}

inline unsigned char bcd2uchar(const unsigned char* pos) {
  return bcd2num(pos, 1);
}

inline unsigned short bcd2ushort(const unsigned char* pos) {
  return bcd2num(pos, 2);
}

inline unsigned int bcd2uint(const unsigned char* pos) {
  return bcd2num(pos, 4);
}

int main() {
  const unsigned int num = 0x30124020;

  const unsigned int year = bcd2ushort((unsigned char*)&num);
  const unsigned int month = bcd2uchar((unsigned char*)&num + 2);
  const unsigned int day = bcd2uchar((unsigned char*)&num + 3);

  std::cout << "year:" << year << std::endl
      << "month:" << month << std::endl
      << "day:" << day << std::endl;
  return EXIT_SUCCESS;
}
