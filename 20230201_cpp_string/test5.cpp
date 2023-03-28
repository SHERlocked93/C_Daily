//
// Created by 钱昱 on 2023/2/27.  C++ primer P362
//
#include <iostream>
#include <iomanip>
#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <iterator>
#include <numeric>
#include <list>
#include <fstream>

using namespace std;



int main() {
    std::list<int> l = {1, 2, 0, 4, 5, 6, 7, 0, 9};2
    auto it = find(l.crbegin(), l.crend(), 0);
    
    cout << distance(it, l.crend()) << endl;
    int *pi = new int;
    
    //copy(list_int.cbegin(), list_int.cend(), ostream_iterator<int>(cout, " "));
    return 0;
}
