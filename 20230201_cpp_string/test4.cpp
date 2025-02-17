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

int main()
{
    istream_iterator<int> in_iter(cin), eof;
    vector<int> vec_int;

    copy(in_iter, eof, back_inserter(vec_int));

    sort(vec_int.begin(), vec_int.end());

    unique_copy(vec_int.cbegin(), vec_int.cend(), ostream_iterator<int>(cout, " "));

    return 0;
}
