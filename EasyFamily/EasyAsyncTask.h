#pragma once
// 改进方向：
// 1. 合并简单定时任务到一个线程中
#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>


namespace thread_task {
class EasyAsyncTask;
class TaskWorkArgsBase {
 public:
  explicit TaskWorkArgsBase(int interval_millseconds = 0)
      : interval_millseconds_(interval_millseconds) {}
  virtual ~TaskWorkArgsBase() = default;
  TaskWorkArgsBase(const TaskWorkArgsBase&) = default;
  TaskWorkArgsBase& operator=(const TaskWorkArgsBase&) = default;

  int interval_millseconds_{0};  // 任务间隔时间 0表示单次任务 ms
};

class TaskResultBase {
 public:
  int retVal_ = -1;   // 0成功 -2超时 其他失败
  std::string desp_;  // 结果描述
  virtual ~TaskResultBase() = default;
};

class TaskProcessBase {
 public:
  int process_ = 0;      // 0开始 大于等于100完成
  std::string message_;  // 进度消息
  virtual ~TaskProcessBase() = default;
};

// 任务取消类型
enum class TaskCancelType : int {
  TIMEOUT,  // 超时取消
  MANUAL,   // 手动取消
};

using TaskWorkLoad = std::function<std::shared_ptr<TaskResultBase>(
    EasyAsyncTask*, std::shared_ptr<TaskWorkArgsBase>)>;
using TaskProcessHandler = std::function<void(TaskProcessBase&)>;
using TaskDoneHandler = std::function<void(std::shared_ptr<TaskResultBase>&)>;
using TaskCancelHandler = std::function<void(EasyAsyncTask*, TaskCancelType)>;

class  EasyAsyncTask {
 public:
  EasyAsyncTask() = default;
  ~EasyAsyncTask() {
    cancel();
    wait();
  }

  //! 设置任务
  EasyAsyncTask* setWorkLoad(const TaskWorkLoad& workLoad);

  //! 设置任务完成回调
  EasyAsyncTask* setTaskDoneHandler(const TaskDoneHandler& dHandler);
  //! 设置任务进度回调
  EasyAsyncTask* setProcessHandler(const TaskProcessHandler& pHandler);
  //! 设置任务取消回调
  EasyAsyncTask* setCancelHandler(const TaskCancelHandler& pHandler);

  //! 启动异步任务
  //! \param args 时间间隔
  EasyAsyncTask* runAsync(const std::shared_ptr<TaskWorkArgsBase>& args);
  //! 设置超时时间 单位ms
  EasyAsyncTask* setTimeoutDuration(int timeout_duration_ms_);

  int cancel();
  void wait() const;

  //! 获取任务结果
  std::shared_ptr<TaskResultBase> getResult();
  //! 上报任务进度
  void reportProcess(TaskProcessBase& process) const;
  //! 获取是否取消
  bool isCancellationRequested() const;

 private:
  TaskWorkLoad task_work_load_ = nullptr;            // 任务
  TaskProcessHandler process_reporter_ = nullptr;    // 进度回调
  TaskDoneHandler task_done_handler_ = nullptr;      // 完成回调
  TaskCancelHandler task_cancel_handler_ = nullptr;  // 取消回调，只循环任务有

  bool is_work_load_bind_ = false;            // 是否绑定任务
  bool is_process_reporter_bind_ = false;     // 是否绑定进度回调
  bool is_task_done_handler_bind_ = false;    // 是否绑定完成回调
  bool is_task_cancel_handler_bind_ = false;  // 是否绑定取消回调

  std::shared_ptr<TaskResultBase> result_obj_;  // 任务结果
  std::shared_ptr<std::thread> thread_;         // 任务线程
  std::shared_ptr<TaskWorkArgsBase> args_{};    // 存储任务参数

  //! 启任务线程
  void privateRunner(EasyAsyncTask* t,
                     const std::shared_ptr<TaskWorkArgsBase>& args);

  // std::condition_variable cv_;
  std::mutex mutex_;
  std::atomic<int> cancel_pending_{0};
  std::atomic<int> is_working_{0};
  std::atomic<bool> is_waiting_{false};
  std::chrono::time_point<std::chrono::system_clock> start_time_;  // 开始时间
  std::chrono::milliseconds timeout_duration_;  // 超时时间 ms 0为不超时

  const std::chrono::milliseconds timeout_check_duration_ =
      std::chrono::milliseconds(20);  // 超时检查间隔 ms
};
}  // namespace thread_task
