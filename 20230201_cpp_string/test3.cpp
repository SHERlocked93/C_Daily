//
// Created by SHERlocked93 on 2023/2/26. C++ primer P362
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

int main()
{
    ifstream ifs("/Users/sherlocked93/Downloads/1.txt");
    vector<string> vec1;
    istream_iterator<string> ifs_iter(ifs), eof;

    copy(ifs_iter, eof, back_inserter(vec1));

    copy(vec1.cbegin(), vec1.cend(), ostream_iterator<string>(cout, "\n"));

    return 0;
}
