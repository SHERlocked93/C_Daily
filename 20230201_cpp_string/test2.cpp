//
// Created by SHERlocked93 on 2023/2/1.
//
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    char a = 0x01;
    char b = 0x03 << 2;

    std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<unsigned int>(a | b);

    return 0;
}
