#include "AsyncTask.h"
using namespace common_util::thread_task;

AsyncTask::AsyncTask() : task_work_load_(), process_reporter_(), task_done_handler_(), result_obj_(nullptr) {}

// bind worker
void AsyncTask::setWorkLoad(const TaskWorkLoad& workLoad) {
  task_work_load_ = workLoad;
  is_work_load_bind_ = true;
}

void AsyncTask::setProcessHandler(const TaskProcessHandler& pHandler) {
  // bind reporter
  process_reporter_ = pHandler;
  is_process_reporter_bind_ = true;
}

void AsyncTask::setTaskDoneHandler(const TaskDoneHandler& dHandler) {
  task_done_handler_ = dHandler;
  is_task_done_handler_bind_ = true;
}

int AsyncTask::runAsync(TaskWorkArgs& args) {
  thread_ = std::make_shared<std::thread>(&AsyncTask::privateRunner, this, this, args);
  return 0;
}

int AsyncTask::cancel() {
  cancel_pending_++;
  return cancel_pending_;
}

int common_util::thread_task::AsyncTask::wait() {
  //{
  //  is_waiting_ = true;
  //  std::unique_lock<std::mutex> lk(mutex_);
  //  cv_.wait(lk);
  //}
  thread_->join();
  return 0;
}

std::shared_ptr<TaskResult> AsyncTask::getResult() { return result_obj_; }

void AsyncTask::reportProcess(TaskProcess& process) { process_reporter_(process); }

bool AsyncTask::isCancellationRequested() { return cancel_pending_ > 0; }

void common_util::thread_task::AsyncTask::privateRunner(AsyncTask* t, TaskWorkArgs args) {
  is_working_ = 1;

  if (is_work_load_bind_) result_obj_ = task_work_load_(t, args);

  is_working_ = 0;

  if (is_task_done_handler_bind_) task_done_handler_(result_obj_);

  // if (is_waiting_) cv_.notify_one();
}
