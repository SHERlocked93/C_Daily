#include "EasyAsyncTask.h"

using namespace thread_task;

EasyAsyncTask* EasyAsyncTask::setWorkLoad(const TaskWorkLoad& workLoad) {
  task_work_load_ = workLoad;
  is_work_load_bind_ = true;
  return this;
}

EasyAsyncTask* EasyAsyncTask::setTaskDoneHandler(const TaskDoneHandler& dHandler) {
  task_done_handler_ = dHandler;
  is_task_done_handler_bind_ = true;
  return this;
}

EasyAsyncTask* EasyAsyncTask::setProcessHandler(const TaskProcessHandler& pHandler) {
  process_reporter_ = pHandler;
  is_process_reporter_bind_ = true;
  return this;
}
EasyAsyncTask* EasyAsyncTask::setCancelHandler(const TaskCancelHandler& pHandler) {
  task_cancel_handler_ = pHandler;
  is_task_cancel_handler_bind_ = true;
  return this;
}

EasyAsyncTask* EasyAsyncTask::setTimeoutDuration(int timeout_duration_ms_) {
  timeout_duration_ = std::chrono::milliseconds(timeout_duration_ms_);
  return this;
}

EasyAsyncTask* EasyAsyncTask::runAsync(const std::shared_ptr<TaskWorkArgsBase>& args) {
  start_time_ = std::chrono::system_clock::now();
  thread_ = std::make_shared<std::thread>(&EasyAsyncTask::privateRunner, this, this,
                                          args);
  return this;
}

int EasyAsyncTask::cancel() { return ++cancel_pending_; }

void EasyAsyncTask::wait() const {
  //{
  //  is_waiting_ = true;
  //  std::unique_lock<std::mutex> lk(mutex_);
  //  cv_.wait(lk);
  //}
  if (thread_ && thread_->joinable()) {
    thread_->join();
  }
}

std::shared_ptr<TaskResultBase> EasyAsyncTask::getResult() {
  std::lock_guard lock(mutex_);
  return result_obj_;
}

void EasyAsyncTask::reportProcess(TaskProcessBase& process) const {
  if (is_process_reporter_bind_) process_reporter_(process);
}

bool EasyAsyncTask::isCancellationRequested() const { return cancel_pending_ > 0; }

void EasyAsyncTask::privateRunner(EasyAsyncTask* t,
                              const std::shared_ptr<TaskWorkArgsBase>& args) {
  is_working_ = 1;

  try {
    bool isLoopWork = args->interval_millseconds_ > 0;
    do {
      if (is_work_load_bind_) {
        result_obj_ = task_work_load_(t, args);
      }

      if (is_task_done_handler_bind_ && result_obj_) {
        std::lock_guard lock(mutex_);
        task_done_handler_(result_obj_);
      }

      if (isLoopWork) {
        auto start_loop = std::chrono::system_clock::now();

        while (std::chrono::system_clock::now() - start_loop <
               std::chrono::milliseconds(args->interval_millseconds_)) {
          if (isCancellationRequested()) {
            if (is_task_cancel_handler_bind_)
              task_cancel_handler_(this, TaskCancelType::MANUAL);
            break;
          }

          // ³¬Ê±¼ì²â
          if (timeout_duration_ > std::chrono::microseconds(0) &&
              std::chrono::system_clock::now() - start_time_ >
                  timeout_duration_) {
            cancel();
            if (is_task_cancel_handler_bind_)
              task_cancel_handler_(this, TaskCancelType::TIMEOUT);
            break;
          }

          std::this_thread::sleep_for(timeout_check_duration_);
        }
      }
    } while (!isCancellationRequested() && isLoopWork);
  } catch (const std::exception& e) {
    result_obj_ = std::make_shared<TaskResultBase>();
    result_obj_->retVal_ = -3;
    result_obj_->desp_ = e.what();
  } catch (...) {
    result_obj_ = std::make_shared<TaskResultBase>();
    result_obj_->retVal_ = -4;
    result_obj_->desp_ = "Unknown error";
  }

  is_working_ = 0;
}
