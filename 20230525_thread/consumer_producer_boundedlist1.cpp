// 有界生产者消费者队列
// Created by 钱昱 on 2025/2/19.
//
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <list>
#include <ranges>
#include <thread>

using namespace std::literals;

template <typename T>
class MyBoundedList {
 public:
  explicit MyBoundedList(const int size) : m_sizeLimit(size) {}

  void put(T&& t) {
    std::unique_lock lock(m_mutex);
    m_notFull.wait(lock, [this] { return m_list.size() <= m_sizeLimit; });
    m_list.emplace_back(t);
    m_notEmpty.notify_all();
  }

  T get() {
    std::unique_lock lock(m_mutex);
    m_notEmpty.wait(lock, [this] { return m_list.size(); });
    auto item = m_list.front();
    m_list.pop_front();

    m_notFull.notify_all();
    return item;
  }

 private:
  int m_sizeLimit;      // 队列容量
  std::mutex m_mutex;   // 互斥锁
  std::list<T> m_list;  // 队列

  std::condition_variable m_notFull;   // 非空条件变量
  std::condition_variable m_notEmpty;  // 非满条件变量
};

int main() {
  MyBoundedList<int> ListTest(10);

  std::thread producer1([&ListTest] {
    for (auto i : std::views::iota(0, 100)) {
      std::this_thread::sleep_for(5ms);
      ListTest.put(std::move(i));

      std::cout << "producer1 id:" << std::this_thread::get_id() << " ->put: " << i << std::endl;
    }
  });
  std::thread producer2([&ListTest] {
    for (auto i : std::views::iota(0, 100)) {
      std::this_thread::sleep_for(5ms);
      ListTest.put(std::move(i));

      std::cout << "producer2 id:" << std::this_thread::get_id() << " ->put: " << i << std::endl;
    }
  });

  std::thread consumer1([&ListTest] {
    for (auto i : std::views::iota(0, 500)) {
      std::this_thread::sleep_for(100ms);

      std::cout << " = consumer1 id:" << std::this_thread::get_id() << " ->get: " << ListTest.get() << std::endl;
    }
  });
  std::thread consumer2([&ListTest] {
    for (auto i : std::views::iota(0, 500)) {
      std::this_thread::sleep_for(200ms);

      std::cout << " = consumer2 id:" << std::this_thread::get_id() << " ->get: " << ListTest.get() << std::endl;
    }
  });

  producer1.join();
  producer2.join();
  consumer1.join();
  consumer2.join();
  return 0;
}
