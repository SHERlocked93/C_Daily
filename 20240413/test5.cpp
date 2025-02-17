#include <iostream>
#include <string>
using namespace std;

class Base {
  std::string name_ = "-";

 public:
  Base() = default;
  // 自定义拷贝构造
  Base(const Base &rhs) : name_(rhs.getName()) {}
  // 自定义拷贝赋值
  Base &operator=(const Base &rhs) {
    name_ = rhs.getName();
    return *this;
  }
  std::string getName() const { return name_; }
  void setName(const std::string &name) { name_ = name; }
};

class A : public Base {  // 派生类
  int member_id_;

 public:
  A() = default;
  // 派生类自定义拷贝构造
  A(const A &rhs) : Base(rhs), member_id_(rhs.getMemberId()) {}
  // 派生类自定义拷贝赋值
  A &operator=(const A &rhs) {
    Base::operator=(rhs);
    member_id_ = rhs.getMemberId();
    return *this;
  }
  int getMemberId() const { return member_id_; }
  void setMemberId(int member_id) { member_id_ = member_id; }
};

int main() {
  A a;
  a.setName("SHERlocked93");
  a.setMemberId(2);  // a "SHERlocked93" 2

  A b(a);  // b "SHERlocked93" 2

  A c = a;  // c "SHERlocked93" 2
}
