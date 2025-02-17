//
// Created by 钱昱 on 2025/2/6.
//

#include <functional>
#include <iostream>

class CTest {
public:
  explicit CTest(int v) : v_(v) {}

  int add(int a, int b) const { return a + b + v_; }

private:
  int v_ = 0;
};


int main() {
  // std::bind一个有意思的用法是把成员函数转换成可调用对象
  CTest m(10);
  CTest n(18);

  const auto funcM = std::bind(&CTest::add, &m, std::placeholders::_1, std::placeholders::_2);
  const auto funcN = std::bind(&CTest::add, &n, std::placeholders::_1, std::placeholders::_2);

  std::cout << "m:" << funcM(1, 2) << std::endl;
  std::cout << "n:" << funcN(1, 2) << std::endl;
}

