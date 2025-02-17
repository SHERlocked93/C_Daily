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
#include <regex>
#include <vector>

using namespace std;

int main()
{
    regex r("\\S+[^c]ei.+", regex::icase);
    smatch re_result;

    string st = "receipt freind theif";

    if (std::regex_search(st, re_result, r))
        cout << re_result.str()
             << std::boolalpha << "-" << true << "-" << false
             << noboolalpha << true << "-" << false << endl;

    return 0;
}
