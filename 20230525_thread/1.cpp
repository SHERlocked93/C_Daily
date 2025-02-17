#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include <bitset>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
typedef unsigned char UCHAR;
using namespace std;
#include <iostream>
#include <vector>

#include <thread>

void fun1(int a)
{
    a = 111;
    cout << "fun1 : a = " << a << endl;
}
void fun2(int& a)
{
    a = 222;
    cout << "fun2 : a = " << a << endl;
}
int main()
{
    int a1 = 1, a2 = 2;
    thread t1(fun1, a1);
    thread t2(fun2, ref(a2));

    t1.join();
    t2.join();
    cout << a1 << endl;
    cout << a2 << endl;
    exit(0);
}
