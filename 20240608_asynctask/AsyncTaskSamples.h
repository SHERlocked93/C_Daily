#pragma once

#include "./AsyncTask.h"

namespace common_util {
namespace thread_task {
class SampleTaskArgs : public TaskWorkArgs {
 public:
  std::string id;  // 唯一识别ID
};
}  // namespace thread_task
}  // namespace common_util

class AsyncTaskSamples {
 public:
  AsyncTaskSamples() = default;

  void runSample();

 private:
  int startEasyTask();

  int startLoopTask();

  int endLoopTask();

  std::shared_ptr<common_util::thread_task::AsyncTask> loop_worker_;

  std::shared_ptr<common_util::thread_task::TaskResult> easyTask(common_util::thread_task::AsyncTask* t,
                                                                 common_util::thread_task::TaskWorkArgs& e);

  std::shared_ptr<common_util::thread_task::TaskResult> loopTask(common_util::thread_task::AsyncTask* t,
                                                                 common_util::thread_task::TaskWorkArgs& e);

  std::string logger_name_;
};
