#include <cmath>
#include <cstring>
#include <bitset>
#include <cstring>
#include <ctime>
#include <limits>
#include <random>
#include <regex>
#include <sstream>
#include <vector>
#include <iostream>
#include <vector>
#include <thread>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

class MyClass {
public:
    std::string data;

    explicit MyClass(std::string _data = "")
        : data(std::move(_data)){};

    // 拷贝构造函数
    MyClass(const MyClass& other)
    {
        cout << "拷贝构造函数" << endl;
        data = other.data;
    }

    // 移动构造函数
    MyClass(MyClass&& other) noexcept
    {
        cout << "移动构造函数  " ;
        data = std::move(other.data);
    }

    // 移动赋值运算符
    MyClass& operator=(MyClass&& other) noexcept
    {
        cout << "移动赋值运算符  " ;
        if (this != &other) {
            data = std::move(other.data);
        }
        return *this;
    }

    // 拷贝赋值运算符
    MyClass& operator=(const MyClass& other)
    {
        cout << "拷贝赋值运算符" << endl;
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }
};

int main()
{
    MyClass A("SHERlocked93");

    MyClass D = std::move(A); // 移动构造
    cout << D.data << endl;

//    MyClass E(std::move(D)); // 移动构造
//    cout << E.data << endl;

    MyClass G;
    G = std::move(D); // 移动赋值
    cout << G.data << endl;
}

