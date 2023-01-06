//
// Created by qiany on 2023/1/6.
//
#include <iostream>
#include <vector>

using namespace std;


int main() {
    char a = 0;
    char strHelloArr[] = "Helloworld";
    char *strHello = strHelloArr;
    
    cout << sizeof(strHello) << endl;
    cout << strHello << endl;
    return 0;
}
