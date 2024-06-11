#include <iostream>
#include <string>
#include <utility>

using namespace std;

class MyClass {
public:
    std::string data;

    explicit MyClass(std::string _data = "")
        : data(std::move(_data)){ cout << "默认构造函数  ";};

    MyClass(const MyClass& other)
    {
        cout << "拷贝构造函数  ";
        data = other.data;
    }

    MyClass(MyClass&& other) noexcept
    {
        cout << "移动构造函数  ";
        data = std::move(other.data);
    }

    MyClass& operator=(MyClass&& other) noexcept
    {
        cout << "移动赋值运算符  ";
        if (this != &other) {
            data = std::move(other.data);
        }
        return *this;
    }

    MyClass& operator=(const MyClass& other)
    {
        cout << "拷贝赋值运算符  ";
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }
};

void func(const MyClass& mc){}

const MyClass operator^ (const  MyClass& lhs,const  MyClass& rhs);


int main()
{
    MyClass a("hello");
//    MyClass b = a;
    func(a);

    cout << endl;
}

// 打印：
// 移动构造函数  移动构造函数  移动构造函数  移动构造函数
// SHERlocked93-hello wrold!
