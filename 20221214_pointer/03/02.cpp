//
// Created by qiany on 2023/1/6.
//
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;


int main() {
    char strHello[] = "Hello1";
    char strHello1[] = "world2";
    
    // 拼接  容易导致缓冲区溢出
    strcat(strHello1, "HHHHHHHHHHHHHHHHHHHHHHHH");
    //strcat_s(strHello1, 20, "HHHHHHHHHHHHHHHHHHHHHHHH");
    cout << strHello << "-" << strHello1 << endl;
    //cout << strlen(strHello) << endl;
    //cout << sizeof(strHello) << endl;
    return 0;
}
