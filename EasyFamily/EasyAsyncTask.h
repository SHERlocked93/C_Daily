#pragma once
// Improvement suggestions:
// 1. Merge simple timer and general thread pool
#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace common_util {

class EasyAsyncTask;
class TaskWorkArgsBase {
 public:
  explicit TaskWorkArgsBase(int interval_millseconds = 0) : interval_millseconds_(interval_millseconds) {}
  virtual ~TaskWorkArgsBase() = default;
  TaskWorkArgsBase(const TaskWorkArgsBase&) = default;
  TaskWorkArgsBase& operator=(const TaskWorkArgsBase&) = default;

  int interval_millseconds_{0};  // Loop interval 0 means no loop, unit: ms
};

class TaskResultBase {
 public:
  int retVal_ = -1;   // 0: success, -2: timeout, others: failure
  std::string desp_;  // Result description
  virtual ~TaskResultBase() = default;
};

class TaskProcessBase {
 public:
  int process_ = 0;      // 0: start, ongoing until 100: complete
  std::string message_;  // Progress message
  virtual ~TaskProcessBase() = default;
};

// Task cancellation type
enum class TaskCancelType : int {
  TIMEOUT,  // Timeout cancellation
  MANUAL,   // Manual cancellation
};

using TaskWorkLoad = std::function<std::shared_ptr<TaskResultBase>(EasyAsyncTask*, std::shared_ptr<TaskWorkArgsBase>)>;
using TaskProcessHandler = std::function<void(TaskProcessBase&)>;
using TaskDoneHandler = std::function<void(std::shared_ptr<TaskResultBase>&)>;
using TaskCancelHandler = std::function<void(EasyAsyncTask*, TaskCancelType)>;

class EasyAsyncTask {
 public:
  EasyAsyncTask() = default;
  ~EasyAsyncTask() {
    cancel();
    wait();
  }

  /// Set workload
  EasyAsyncTask* setWorkLoad(const TaskWorkLoad& workLoad);

  /// Set task done callback
  EasyAsyncTask* setTaskDoneHandler(const TaskDoneHandler& dHandler);
  /// Set process callback
  EasyAsyncTask* setProcessHandler(const TaskProcessHandler& pHandler);
  /// Set cancellation callback
  EasyAsyncTask* setCancelHandler(const TaskCancelHandler& pHandler);

  /// Run async task
  /// \param args Task arguments
  EasyAsyncTask* runAsync(const std::shared_ptr<TaskWorkArgsBase>& args);
  /// Set timeout duration, unit: ms
  EasyAsyncTask* setTimeoutDuration(int timeout_duration_ms_);

  int cancel();
  void wait() const;

  /// Get result
  std::shared_ptr<TaskResultBase> getResult();
  /// Report progress
  void reportProcess(TaskProcessBase& process) const;
  /// Check if cancellation is requested
  bool isCancellationRequested() const;

 private:
  TaskWorkLoad task_work_load_ = nullptr;            // Task workload
  TaskProcessHandler process_reporter_ = nullptr;    // Progress callback
  TaskDoneHandler task_done_handler_ = nullptr;      // Done callback
  TaskCancelHandler task_cancel_handler_ = nullptr;  // Cancellation callback, only cyclic tasks available

  bool is_work_load_bind_ = false;            // Whether workload is bound
  bool is_process_reporter_bind_ = false;     // Whether progress callback is bound
  bool is_task_done_handler_bind_ = false;    // Whether done callback is bound
  bool is_task_cancel_handler_bind_ = false;  // Whether cancellation callback is bound

  std::shared_ptr<TaskResultBase> result_obj_;  // Result object
  std::shared_ptr<std::thread> thread_;         // Working thread
  std::shared_ptr<TaskWorkArgsBase> args_{};    // Store task arguments

  /// Working thread
  void privateRunner(EasyAsyncTask* t, const std::shared_ptr<TaskWorkArgsBase>& args);

  // std::condition_variable cv_;
  std::mutex mutex_;
  std::atomic<int> cancel_pending_{0};
  std::atomic<int> is_working_{0};
  std::atomic<bool> is_waiting_{false};
  std::chrono::time_point<std::chrono::system_clock> start_time_;  // Start time
  std::chrono::milliseconds timeout_duration_;                     // Timeout duration ms, 0 means no timeout

  const std::chrono::milliseconds timeout_check_duration_ = std::chrono::milliseconds(20);  // Timeout check interval ms
};
}  // namespace thread_task
