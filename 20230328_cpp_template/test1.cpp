//
// Created by SHERlocked93 on 2023/2/27.  C++ primer P362
//
#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <vector>

using namespace std;

template <unsigned N, unsigned M>
int compare(const char (&p1)[N], const char (&p2)[M])
{
    return strcmp(p1, p2);
}

template <typename T>
class Blob {
public:
    typedef T value_type;
    typedef typename vector<T>::size_type size_type;
    T& back();
};

int main()
{
    bool res = compare("ABC", "DEF");

    cout << " " << res << "/" << strcmp("ABC", "DEF") << endl;

    return 0;
}
