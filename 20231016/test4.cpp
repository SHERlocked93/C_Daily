#include <iostream>
#include <string>
using namespace std;

class MyClass {
public:
    int data;
    explicit MyClass(int _data = 5)
        : data(_data){};

    // 拷贝构造函数
    MyClass(const MyClass& other)
    {
        cout << "拷贝构造函数" << endl;
        data = other.data;
    }

    // 移动构造函数
    MyClass(MyClass&& other) noexcept
    {
        cout << "移动构造函数" << endl;
        data = other.data;
        other.data = 0;
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

    // 移动赋值运算符
    MyClass& operator=(MyClass&& other) noexcept
    {
        cout << "移动赋值运算符" << endl;
        if (this != &other) {
            data = other.data;
            other.data = 0;
        }
        return *this;
    }
};
int main()
{
    int i = 10;
    int& j = i;             // 正确：左值引用
                            //    int& k = i * 1;       // 错误：左值引用不能绑定右值
    int&& m = i * 1;        // 正确：右值引用
                            //    int&& n = i;          // 错误：右值引用不能绑定左值
    const int& p = i * 1;   // 正确：const左值引用可以绑定右值
    int&& q = std::move(m); // 正确：std::move可以将左值转换为右值

    cout << j << endl;

    MyClass A(1);

    MyClass B(A); // 拷贝构造
    cout << B.data << endl;

    MyClass C = A; // 拷贝构造
    cout << C.data << endl;

    MyClass D = std::move(A); // 移动构造
    cout << D.data << endl;

    MyClass E(std::move(D)); // 移动构造
    cout << E.data << endl;

    MyClass F;
    F = E; // 拷贝赋值
    cout << F.data << endl;

    MyClass G;
    G = std::move(E); // 移动赋值
    cout << G.data << endl;
}
