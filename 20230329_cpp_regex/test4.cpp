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

class Class {};
class Base : virtual public Class {};
class D1 : virtual public Base {};
class D2 : virtual public Base {};
class MI : public D1, public D2 {};
class Final : public MI, public Class {};

int main()
{
    Class* pc;
    pc = dynamic_cast<MI*>(new Final);

    return 1;
}
