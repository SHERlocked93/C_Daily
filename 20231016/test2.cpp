#include <bitset>
#include <cmath>
#include <cstring>
#include <ctime>
// #include <iomanip>
#include <limits>
#include <random>
#include <regex>
// #include <sstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

using namespace std;

class MyClass {
public:
    std::string data;

    explicit MyClass(std::string _data = "")
        : data(std::move(_data)){};

    // 拷贝构造函数
    MyClass(const MyClass& other)
    {
        cout << "拷贝构造函数  ";
        data = other.data;
    }

    // 移动构造函数
    MyClass(MyClass&& other) noexcept
    {
        cout << "移动构造函数  ";
        data = std::move(other.data);
    }

    // 移动赋值运算符
    MyClass& operator=(MyClass&& other) noexcept
    {
        cout << "移动赋值运算符  ";
        if (this != &other) {
            data = std::move(other.data);
        }
        return *this;
    }

    // 拷贝赋值运算符
    MyClass& operator=(const MyClass& other)
    {
        cout << "拷贝赋值运算符  ";
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }
};

template <typename T>
std::vector<T> concat(std::vector<T>& vec1,
                      std::vector<T>& vec2)
{
    std::vector<T> res;

    res.reserve(vec1.size() + vec2.size());

    res.insert(res.end(),
               std::make_move_iterator(vec1.begin()),
               std::make_move_iterator(vec1.end()));

    res.insert(res.end(),
               std::make_move_iterator(vec2.begin()),
               std::make_move_iterator(vec2.end()));

    return res;
}

int main()
{
    vector<MyClass> a;
    vector<MyClass> b;
    a.emplace_back(MyClass("SHERlocked93"));
    b.emplace_back(MyClass("hello wrold!"));

    cout << endl;
    auto r = concat(a, b);

    cout << endl;
    cout << r[0].data << "-" << r[1].data << endl;
}
