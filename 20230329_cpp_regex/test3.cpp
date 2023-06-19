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

namespace A {
    int i = 1;

    void print_i()
    {
        cout << i << endl;
    }
} // namespace A

int main()
{
    int i = 3;

    A::print_i();
    return 1;
}
