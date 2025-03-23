// 有界生产者消费者队列
// Created by 钱昱 on 2025/2/19.
//
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <list>
#include <random>
#include <ranges>
#include <thread>

using namespace std::literals;

class MyTaskPayload {
 public:
  MyTaskPayload(std::vector<unsigned char> m_payload) : m_payload(std::move(m_payload)) { m_id++; }

  //! get result
  //! \return result from connection
  std::vector<unsigned char>&& getRes() { return std::move(m_res); }

 private:
  static int m_id;
  std::vector<unsigned char> m_payload;
  std::vector<unsigned char> m_res;
};
int MyTaskPayload::m_id = 0;

template <typename T>
class MyBoundedList {
 public:
  explicit MyBoundedList(const int size) : m_sizeLimit(size) {}

  void put(T&& t) {
    std::unique_lock lock(m_mutex);
    m_notFull.wait(lock, [this] { return m_list.size() <= m_sizeLimit; });
    m_list.emplace_back(t);
    m_notEmpty.notify_one();
  }

  T get() {
    std::unique_lock lock(m_mutex);
    m_notEmpty.wait(lock, [this] { return m_list.size(); });
    auto item = std::move(m_list.front());
    m_list.pop_front();

    m_notFull.notify_one();
    return std::move(item);
  }

 private:
  int m_sizeLimit;      // 队列容量
  std::mutex m_mutex;   // 互斥锁
  std::list<T> m_list;  // 队列

  std::condition_variable m_notFull;   // 非空条件变量
  std::condition_variable m_notEmpty;  // 非满条件变量
};

constexpr unsigned char STX = 0xAA;
constexpr unsigned char EDL = 0xDD;
std::vector<unsigned char> geneRandomData() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned char> dis(0, 0xFF);
  return {STX, dis(gen), dis(gen), dis(gen), EDL};
}

int main() {
  MyBoundedList<MyTaskPayload> ListTest(10);

  std::thread producer1([&ListTest] {
    for (auto i : std::views::iota(0, 100) | std::views::transform([](int) { return geneRandomData(); })) {
      std::this_thread::sleep_for(5ms);
      ListTest.put(i);

      std::cout << "producer1 id:" << std::this_thread::get_id() << " ->put: " << i[0] << std::endl;
    }
  });
   std::thread producer2([&ListTest] {
     for (auto i : std::views::iota(0, 100) | std::views::transform([](int) { return geneRandomData(); })) {
       std::this_thread::sleep_for(5ms);
          ListTest.put(i);

          std::cout << "producer1 id:" << std::this_thread::get_id() << " ->put: " << (int)i[1] << std::endl;
     }
   });

  std::thread consumer1([&ListTest] {
    for (auto i : std::views::iota(0, 500)) {
      std::this_thread::sleep_for(100ms);

      std::cout << " = consumer1 id:" << std::this_thread::get_id() << " ->get: " << ListTest.get().getRes().size() << std::endl;
    }
  });
   std::thread consumer2([&ListTest] {
     for (auto i : std::views::iota(0, 500)) {
       std::this_thread::sleep_for(200ms);

       std::cout << " = consumer2 id:" << std::this_thread::get_id() << " ->get: " << ListTest.get().getRes().size() << std::endl;
     }
   });

  producer1.join();
  // producer2.join();
  consumer1.join();
  // consumer2.join();
  return 0;
}
