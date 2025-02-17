#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
using namespace std;

int get_count() {
  static int count = 0;
  return ++count;
}

class task {
public:
  task(int data) : data_(data) {}

  auto lazy_launch() {
    return
        [*this, cut1 = get_count()]()
    mutable {
      ostringstream oss;
      oss << "Done work " << data_
          << " (No. " << cut1
          << ") in thread "
          << this_thread::get_id()
          << '\n';
      msg_ = oss.str();
      calculate();
    };
  }

  void calculate() {
    this_thread::sleep_for(100ms);
    cout << msg_;
  }

private:
  int data_;
  string msg_;
};

int main() {
  auto t = task{37};
  std::cout << "-0" << std::endl;
  thread t1{t.lazy_launch()};
  thread t2{t.lazy_launch()};
  std::cout << "-1" << std::endl;
  t1.join();
  t2.join();
  std::cout << "-2" << std::endl;
}

