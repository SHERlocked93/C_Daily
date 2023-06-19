#include <algorithm>
#include <bitset>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <numeric>
#include <regex>
#include <vector>

using namespace std;

class Screen {
public:
    typedef string::size_type pos;
    static const string Screen::*data() { return &Screen::contents; }

    char get_cursor() const { return contents[cursor]; }
    char get() const;
    char get(pos ht, pos wd) const;

private:
    string contents;
    pos cursor;
    pos height, width;
};

class Screen1 {
public:
    typedef string::size_type pos;
    static shared_ptr<const string> data() { return make_shared<const string>(Screen1::contents); }

    char get_cursor() const { return contents[cursor]; }
    char get() const;
    char get(pos ht, pos wd) const;

private:
    string contents; // 添加 static 关键字
    pos cursor;
    pos height, width;
};

int main()
{
    // 使用智能指针访问 contents
    auto contents_ptr = Screen1::data();
    cout << *contents_ptr << endl; // 输出 "Hello, world!"

    return 0;
}

