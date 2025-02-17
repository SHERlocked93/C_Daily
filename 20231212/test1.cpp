//
// Created by 钱昱 on 2024/3/16.
//

#include <iostream>
#include <string>

class B {
public:
    //    explicit B(const int num, const std::string& name)
    //    {
    //        m_num = num;
    //        m_name = name;
    //    }
    explicit B(const int num, const std::string& name)
        : m_num(num), m_name(name) {}
    int m_num = 0;
    std::string m_name = "default";
    const std::string m_class_name = "B";
};

int main()
{
    B c(1, "c");
    B b = c;
    return 0;
}
