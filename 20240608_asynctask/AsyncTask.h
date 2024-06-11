#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace common_util {
namespace thread_task {

class AsyncTask;

class TaskWorkArgs {
 public:
  virtual ~TaskWorkArgs() {}
};

class TaskResult {
 public:
  int retVal;
  std::string desp;
  virtual ~TaskResult() {}
};

class TaskProcess {
 public:
  int process;
  std::string message;
  virtual ~TaskProcess() {}
};

typedef std::function<std::shared_ptr<TaskResult>(AsyncTask*, TaskWorkArgs&)> TaskWorkLoad;
typedef std::function<void(TaskProcess&)> TaskProcessHandler;
typedef std::function<void(std::shared_ptr<TaskResult>&)> TaskDoneHandler;

class AsyncTask {
 public:
  AsyncTask();
  void setWorkLoad(const TaskWorkLoad& workLoad);
  void setTaskDoneHandler(const TaskDoneHandler& dHandler);
  void setProcessHandler(const TaskProcessHandler& pHandler);

  int runAsync(TaskWorkArgs& args);
  int cancel();
  int wait();

  std::shared_ptr<TaskResult> getResult();

  void reportProcess(TaskProcess& process);
  bool isCancellationRequested();

 private:
  TaskWorkLoad task_work_load_;
  TaskProcessHandler process_reporter_;
  TaskDoneHandler task_done_handler_;

  bool is_work_load_bind_ = false;
  bool is_process_reporter_bind_ = false;
  bool is_task_done_handler_bind_ = false;

  std::shared_ptr<TaskResult> result_obj_;
  std::shared_ptr<std::thread> thread_;

  void privateRunner(AsyncTask* t, TaskWorkArgs args);

  std::condition_variable cv_;
  std::mutex mutex_;
  std::atomic<int> cancel_pending_{0};
  std::atomic<int> is_working_{0};
  std::atomic<bool> is_waiting_{false};
};

}  // namespace thread_task
}  // namespace common_util
