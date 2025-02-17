#include <iostream>
#include <string>
#include <utility>

using namespace std;

class MyClass {
public:
    std::string data;

    explicit MyClass(std::string _data = "")
        : data(std::move(_data)){ cout << "Ĭ�Ϲ��캯��  ";};

    MyClass(const MyClass& other)
    {
        cout << "�������캯��  ";
        data = other.data;
    }

    MyClass(MyClass&& other) noexcept
    {
        cout << "�ƶ����캯��  ";
        data = std::move(other.data);
    }

    MyClass& operator=(MyClass&& other) noexcept
    {
        cout << "�ƶ���ֵ�����  ";
        if (this != &other) {
            data = std::move(other.data);
        }
        return *this;
    }

    MyClass& operator=(const MyClass& other)
    {
        cout << "������ֵ�����  ";
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

// ��ӡ��
// �ƶ����캯��  �ƶ����캯��  �ƶ����캯��  �ƶ����캯��
// SHERlocked93-hello wrold!
