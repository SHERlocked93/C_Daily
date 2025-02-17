#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace common_util {
namespace thread_task {
class AsyncTask;
class TaskWorkArgsBase {
 public:
  explicit TaskWorkArgsBase(int interval_seconds = 0) : interval_seconds_(interval_seconds) {}
  virtual ~TaskWorkArgsBase() = default;
  TaskWorkArgsBase(const TaskWorkArgsBase&) = default;
  TaskWorkArgsBase& operator=(const TaskWorkArgsBase&) = default;

  int interval_seconds_{0};  // 任务间隔时间 0表示单次任务
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

typedef std::function<std::shared_ptr<TaskResultBase>(AsyncTask*, std::shared_ptr<TaskWorkArgsBase>)>
    TaskWorkLoad;
typedef std::function<void(TaskProcessBase&)> TaskProcessHandler;
typedef std::function<void(std::shared_ptr<TaskResultBase>&)> TaskDoneHandler;

class AsyncTask {
 public:
  AsyncTask() = default;
  ~AsyncTask() {
    if (thread_ && thread_->joinable()) {
      cancel();
      thread_->join();
    }
  }

  //! 设置任务
  void setWorkLoad(const TaskWorkLoad& workLoad);

  //! 设置任务完成回调
  void setTaskDoneHandler(const TaskDoneHandler& dHandler);
  //! 设置任务进度回调
  void setProcessHandler(const TaskProcessHandler& pHandler);
  //! 启动异步任务
  int runAsync(const std::shared_ptr<TaskWorkArgsBase>& args);

  int cancel();
  int wait() const;

  //! 获取任务结果
  std::shared_ptr<TaskResultBase> getResult();
  //! 上报任务进度
  void reportProcess(TaskProcessBase& process) const;
  //! 获取是否取消
  bool isCancellationRequested() const;

 private:
  TaskWorkLoad task_work_load_;          // 任务
  TaskProcessHandler process_reporter_;  // 进度回调
  TaskDoneHandler task_done_handler_;    // 完成回调

  bool is_work_load_bind_ = false;          // 是否绑定任务
  bool is_process_reporter_bind_ = false;   // 是否绑定进度回调
  bool is_task_done_handler_bind_ = false;  // 是否绑定完成回调

  std::shared_ptr<TaskResultBase> result_obj_;  // 任务结果
  std::shared_ptr<std::thread> thread_;         // 任务线程
  std::shared_ptr<TaskWorkArgsBase> args_{};    // 存储任务参数

  //! 启任务线程
  void privateRunner(AsyncTask* t, const std::shared_ptr<TaskWorkArgsBase>& args);

  // std::condition_variable cv_;
  std::mutex mutex_;
  std::atomic<int> cancel_pending_{0};
  std::atomic<int> is_working_{0};
  std::atomic<bool> is_waiting_{false};
};
}  // namespace thread_task
}  // namespace common_util

// .h

using namespace common_util::thread_task;

void AsyncTask::setWorkLoad(const TaskWorkLoad& workLoad) {
  task_work_load_ = workLoad;
  is_work_load_bind_ = true;
}

void AsyncTask::setTaskDoneHandler(const TaskDoneHandler& dHandler) {
  task_done_handler_ = dHandler;
  is_task_done_handler_bind_ = true;
}

void AsyncTask::setProcessHandler(const TaskProcessHandler& pHandler) {
  process_reporter_ = pHandler;
  is_process_reporter_bind_ = true;
}

int AsyncTask::runAsync(const std::shared_ptr<TaskWorkArgsBase>& args) {
  thread_ = std::make_shared<std::thread>(&AsyncTask::privateRunner, this, this, args);
  return 0;
}

int AsyncTask::cancel() { return ++cancel_pending_; }

int AsyncTask::wait() const {
  //{
  //  is_waiting_ = true;
  //  std::unique_lock<std::mutex> lk(mutex_);
  //  cv_.wait(lk);
  //}
  thread_->join();
  return 0;
}

std::shared_ptr<TaskResultBase> AsyncTask::getResult() {
  std::lock_guard<std::mutex> lock(mutex_);
  return result_obj_;
}

void AsyncTask::reportProcess(TaskProcessBase& process) const {
  if (is_process_reporter_bind_) process_reporter_(process);
}

bool AsyncTask::isCancellationRequested() const { return cancel_pending_ > 0; }

void AsyncTask::privateRunner(AsyncTask* t, const std::shared_ptr<TaskWorkArgsBase>& args) {
  is_working_ = 1;

  try {
    do {
      if (is_work_load_bind_) {
        result_obj_ = task_work_load_(t, args);
      }

      if (is_task_done_handler_bind_ && result_obj_) {
        std::lock_guard<std::mutex> lock(mutex_);
        task_done_handler_(result_obj_);
      }

      if (args->interval_seconds_ > 0)
        std::this_thread::sleep_for(std::chrono::seconds(args->interval_seconds_));
    } while (!isCancellationRequested() && args->interval_seconds_ > 0);
  } catch (const std::exception& e) {
    result_obj_ = std::make_shared<TaskResultBase>();
    result_obj_->retVal_ = -1;
    result_obj_->desp_ = e.what();
  } catch (...) {
    result_obj_ = std::make_shared<TaskResultBase>();
    result_obj_->retVal_ = -1;
    result_obj_->desp_ = "Unknown error";
  }

  is_working_ = 0;
}
// .cpp 以下为main.cpp

// 自定义任务参数类
class MyTaskArgs : public common_util::thread_task::TaskWorkArgsBase {
 public:
  int data;
  explicit MyTaskArgs(int d, int interval_seconds = 0) : data(d) { interval_seconds_ = interval_seconds; }
};

// 自定义任务结果类
class MyTaskResult : public common_util::thread_task::TaskResultBase {
 public:
  int result = 0;
};

int main() {
  using namespace common_util::thread_task;

#pragma region simple task
  //  // 创建任务对象
  //  AsyncTask simpleTask;
  //
  //  // 设置工作负载
  //  simpleTask.setWorkLoad([](AsyncTask* task, const std::shared_ptr<TaskWorkArgsBase>& args) {
  //    auto myArgs = std::static_pointer_cast<MyTaskArgs>(args);
  //    auto result = std::make_shared<MyTaskResult>();
  //
  //    //    for (int i = 0; i <= 100; i += 20) {
  //    //      std::this_thread::sleep_for(std::chrono::milliseconds(100));
  //    //      TaskProcess process;
  //    //      process.process = i;
  //    //      process.message = "Progress: " + std::to_string(i) + "%";
  //    //      task->reportProcess(process);
  //    //
  //    //      if (task->isCancellationRequested()) {
  //    //        result->retVal = -1;
  //    //        result->desp = "Task cancelled";
  //    //        return result;
  //    //      }
  //    //    }
  //
  //    result->result = myArgs->data * 2;  // 假设计算任务是将输入数据乘以2
  //    result->retVal_ = 0;
  //    result->desp_ = "Task completed successfully";
  //    return result;
  //  });
  //
  //  simpleTask.setProcessHandler([](TaskProcessBase& process) { std::cout << process.message_ << std::endl;
  //  });
  //
  //  simpleTask.setTaskDoneHandler([](std::shared_ptr<TaskResultBase>& result) {
  //    auto myResult = std::dynamic_pointer_cast<MyTaskResult>(result);
  //    if (result->retVal_ == 0) {
  //      std::cout << "Simple Task result: " << myResult->result << std::endl;
  //    } else {
  //      std::cout << "Simple Task error: " << result->desp_ << std::endl;
  //    }
  //  });
  //  simpleTask.runAsync(std::make_shared<MyTaskArgs>(1));
  //
  //  simpleTask.setTaskDoneHandler([](std::shared_ptr<TaskResultBase>& result) {
  //    auto myResult = std::dynamic_pointer_cast<MyTaskResult>(result);
  //    if (result->retVal_ == 0) {
  //      std::cout << "Simple Task result: " << myResult->result << std::endl;
  //    } else {
  //      std::cout << "Simple Task error: " << result->desp_ << std::endl;
  //    }
  //  });

#pragma endregion

#pragma region loop task
//  AsyncTask loopTask;
//
//  loopTask.setWorkLoad([](AsyncTask* task, const std::shared_ptr<TaskWorkArgsBase>& args) {
//    auto myArgs = std::static_pointer_cast<MyTaskArgs>(args);
//
//    std::clog << "[loopTask] loopTask start! id:" << std::this_thread::get_id() << std::endl;
//
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    myArgs->data += 35;
//    std::clog << "[loopTask] loopTask is working... data:" << myArgs->data << std::endl;
//
//    auto result = std::make_shared<MyTaskResult>();
//    result->result = myArgs->data;
//    result->retVal_ = 0;
//    result->desp_ = " Loop task already done!";
//
//    std::clog << "[loopTask] loopTask end!  id:" << std::this_thread::get_id() << std::endl;
//    return result;
//  });
//
//  loopTask.setTaskDoneHandler([](std::shared_ptr<TaskResultBase>& result) {
//    auto myResult = std::dynamic_pointer_cast<MyTaskResult>(result);
//    if (result->retVal_ == 0) {
//      std::cout << "Loop Task result: " << myResult->result << std::endl;
//    } else {
//      std::cout << "Loop Task error: " << result->desp_ << std::endl;
//    }
//  });
//
//  loopTask.runAsync(std::make_shared<MyTaskArgs>(10, 1));
//
//  auto loopTaskResult = loopTask.getResult();
//  if (loopTaskResult) {
//    std::cout << "Final Loop Task Result: retVal = " << loopTaskResult->retVal_
//              << ", desp = " << loopTaskResult->desp_ << std::endl;
//  } else {
//    std::cout << "Failed to Recv Loop Task Result!" << std::endl;
//  }
#pragma endregion

  using namespace common_util::thread_task;

  // if (!conn_state_)  return;
  AsyncTask uploadTransLoopTask;

  uploadTransLoopTask.setWorkLoad([](AsyncTask* task, const std::shared_ptr<TaskWorkArgsBase>& args) {
    std::cout << "setWorkLoad" << std::endl;
    return std::make_shared<TaskResultBase>();
  });
  uploadTransLoopTask.setTaskDoneHandler(
      [](std::shared_ptr<TaskResultBase>& result) { std::cout << "done" << std::endl; });

  uploadTransLoopTask.runAsync(std::make_shared<TaskWorkArgsBase>(1));

  std::this_thread::sleep_for(std::chrono::seconds(10));

  return 0;
}
