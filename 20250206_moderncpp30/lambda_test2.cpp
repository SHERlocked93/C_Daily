#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <numeric>
#include <string>
#include <functional>
#include <vector>


using namespace std;


int main() {
  vector vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  const auto prnStr = [](const string& accStr, const int x) { return accStr + "-" + to_string(x); };

  const auto acc = accumulate(vec.cbegin(), vec.cend(), ""s, prnStr);

  std::cout << "acc:" << acc << std::endl;

  const auto product = accumulate(vec.cbegin(), vec.cend(), 1, multiplies());
  std::cout << "product:" << product << std::endl;
}
