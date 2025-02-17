#include <iostream>
using namespace std;

class A {
 public:
  A(int val_) : val(val_) {}
  int getVal() const { return val; }

  A& operator=(const A& rhs) {
    cout << "A::operator=" << endl;
    val = rhs.getVal();
    return *this;
  }

  A& operator+=(A& rhs) {
    cout << "A::operator+=" << endl;
    val += rhs.getVal();
    return *this;
  }

 private:
  int val;
};

int main() {
  A a(2), b(3), c(4);
  b += a += c;  // b: 9, a: 6, c: 4
  b = a = c;    // a: 4, c: 4, b: 4
}
