#include <iostream>
#include <string>
#include "../EasyObserver.h"

int main() {
  // 示例 1: bool 类型的观察者
  std::cout << "=== Testing MyObserver<bool> ===" << std::endl;
  MyObserver<bool> myBoolFlag;
  
  // 初始化 (此时还没有设置回调，所以不会触发通知)
  myBoolFlag = false;

  // 设置值改变的回调函数
  myBoolFlag.setValueChangeCallback([&](bool receiveAck) {
    std::cout << "Value changed to: " << (receiveAck ? "true" : "false") << std::endl;
  });

  // 改变值，应该触发回调
  std::cout << "Assigning true..." << std::endl;
  myBoolFlag = true;

  // 再次改变值，应该触发回调
  std::cout << "Assigning false..." << std::endl;
  myBoolFlag = false;

  // 值相同，不应该触发回调
  std::cout << "Assigning false again..." << std::endl;
  myBoolFlag = false;


  // 示例 2: int 类型的观察者
  std::cout << "\n=== Testing MyObserver<int> ===" << std::endl;
  MyObserver<int> myIntObserver;

  myIntObserver.setValueChangeCallback([](int newVal) {
    std::cout << "Int value updated: " << newVal << std::endl;
  });

  myIntObserver = 42;
  myIntObserver = 100;
  
  return 0;
}
