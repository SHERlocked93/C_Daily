//
// Created by qiany on 2023/1/6.
//
#include <iostream>
#include <vector>

using namespace std;

istream &func(istream &is)
{
    string buf;
    while (is >> buf) {
        cout << buf << endl;
    }
    is.clear();
    return is;
}

int main()
{
    istream &is = func(cin);
    
    cout << is.rdstate() << endl;
    
    return 0;
}
