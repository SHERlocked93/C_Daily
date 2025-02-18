#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

using SendCb = std::function<void(const std::string&, const std::vector<unsigned char>&)>;

std::mutex rq_mtx;  // recv_queue操作锁
std::mutex cv_mtx;
std::mutex cb_mtx;
std::deque<std::pair<std::string, std::vector<unsigned char>>> recv_queue;
std::unordered_map<std::string, std::condition_variable> cv_map;
std::unordered_map<std::string, SendCb> cb_map;

void sendCommand(const std::string& command) { std::cout << " Sending command: " << command << std::endl; }

// 模拟接收消息并放入队列，此处代码无法更改，为外部代码
void simulateReceiveMessage(const std::string& uniqueID, const std::vector<unsigned char>& message) {
  std::lock_guard lock(rq_mtx);
  recv_queue.emplace_back(uniqueID, message);
}

// ===== 以上为模拟通信库工作 =====

void checkScMsgWorker() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 控制检查频率

    std::unique_lock lock_rq(rq_mtx);
    if (recv_queue.empty()) continue;
    auto message = recv_queue.front();
    std::string uniqueID = message.first;

    // 没有对应唯一ID的回调，打印并丢弃
    {
      std::lock_guard lock_cb(cb_mtx);
      if (!cb_map.contains(uniqueID)) {
        std::cout << "No callback for uniqueID: " << uniqueID << std::endl;
        recv_queue.pop_front();
        continue;
      }
    }
    lock_rq.unlock();

    {
      std::lock_guard lock_cv(cv_mtx);
      cv_map[uniqueID].notify_all();
    }
  }
}

// 发送线程函数
void registCb(const std::string& uniqueID, int timeoutSeconds, SendCb cb) {
  {
    std::lock_guard lock_cb(cb_mtx);
    cb_map[uniqueID] = std::move(cb);
  }
  std::thread([=]() {
    bool isTimeout = false;
    {
      std::unique_lock lock_cv(cv_mtx);
      isTimeout = !cv_map[uniqueID].wait_for(lock_cv, std::chrono::seconds(timeoutSeconds), [&] {
        std::lock_guard lock_rq(rq_mtx);
        return std::any_of(recv_queue.begin(), recv_queue.end(), [&](const auto& item) { return item.first == uniqueID; });
      });
    }

    if (isTimeout) {
      std::cout << "Timeout waiting for response to " << uniqueID << std::endl;
    } else {
      // 获取并移除响应消息
      std::vector<unsigned char> msg;
      {
        std::lock_guard lock(rq_mtx);
        auto it = std::find_if(recv_queue.begin(), recv_queue.end(), [&](const auto& p) { return p.first == uniqueID; });
        if (it != recv_queue.end()) {
          msg = std::move(it->second);
          recv_queue.erase(it);
        }
      }

      // 模拟消费
      {
        std::lock_guard lock_cb(cb_mtx);
        if (cb_map.contains(uniqueID)) {
          cb_map[uniqueID](uniqueID, msg);
          cb_map.erase(uniqueID);
        }
      }
    }

    {
      std::lock_guard lock(cv_mtx);
      cv_map.erase(uniqueID);
    }
  }).detach();
}

int main() {
  std::thread checker(checkScMsgWorker);

  auto printCmd = [](const std::string& id, const std::vector<unsigned char>& response) {
    std::cout << " -> Callback for " << id << ": <";
    for (auto ch : response) {
      std::cout << " 0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(ch);
    }
    std::cout << " >" << std::endl;
  };

  registCb("uniqueID1", 3, printCmd);
  registCb("uniqueID3", 3, printCmd);
  registCb("uniqueID2", 3, printCmd);
  registCb("uniqueID4", 3, printCmd);
  registCb("uniqueID5", 3, printCmd);
  registCb("uniqueID6", 3, printCmd);
  registCb("uniqueID7", 3, printCmd);
  registCb("uniqueID8", 3, printCmd);

  // 模拟接收消息
  std::thread receiver([] {
    simulateReceiveMessage("uniqueID7", {'r', 'e', 'c', 'v', 2, 'f', 'o', 'r', '7'});
    std::this_thread::sleep_for(std::chrono::seconds(2));
    simulateReceiveMessage("uniqueID1", {'r', 'e', 'c', 'v', 2, 'f', 'o', 'r', '1'});
    simulateReceiveMessage("uniqueID3", {'r', 'e', 'c', 'v', 2, 'f', 'o', 'r', '3'});
    simulateReceiveMessage("uniqueID4", {'r', 'e', 'c', 'v', 2, 'f', 'o', 'r', '4'});
    simulateReceiveMessage("uniqueID5", {'r', 'e', 'c', 'v', 2, 'f', 'o', 'r', '5'});
    simulateReceiveMessage("uniqueID6", {'r', 'e', 'c', 'v', 2, 'f', 'o', 'r', '6'});
    simulateReceiveMessage("uniqueID9", {'r', 'e', 'c', 'v', 2, 'f', 'o', 'r', '9'});

    std::this_thread::sleep_for(std::chrono::milliseconds(950));
    simulateReceiveMessage("uniqueID2", {'r', 'e', 'c', 'v', 3, 'f', 'o', 'r', '2'});
    std::this_thread::sleep_for(std::chrono::milliseconds(550));
    simulateReceiveMessage("uniqueID8", {'r', 'e', 'c', 'v', 3, 'f', 'o', 'r', '8'});
  });

  checker.join();
  receiver.join();

  return 0;
}
