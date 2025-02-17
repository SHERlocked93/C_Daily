#include <iostream>
using namespace std;

struct TransBase {
  TransBase() { logTrans(); }
  virtual void logTrans() { cout << "TransBase" << endl; }
};

struct TransA : TransBase {
  TransA() { logTrans(); }
  virtual void logTrans() { std::cout << "TransA" << std::endl; }
};

struct TransB : TransBase {
  TransB() { logTrans(); }

  virtual void logTrans() { std::cout << "TransB" << std::endl; }
};

int main() {
  TransA a;
  TransB b;
}
