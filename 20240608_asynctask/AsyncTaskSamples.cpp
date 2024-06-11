#include "AsyncTaskSamples.h"

#include <chrono>
#include <iostream>

using namespace common_util::thread_task;

AsyncTaskSamples::AsyncTaskSamples() {}

void AsyncTaskSamples::runSample() {
  startEasyTask();

  startLoopTask();
  endLoopTask();
}

int AsyncTaskSamples::startEasyTask() {
  SampleTaskArgs a;
  a.id = 18;

  AsyncTask at;
  std::clog << "[startEasyTask] AsyncTask constructed";
  at.setWorkLoad(std::bind(&AsyncTaskSamples::easyTask, this, std::placeholders::_1, std::placeholders::_2));
  std::clog << "[startEasyTask] AsyncTask workload assigned";
  at.runAsync(a);
  std::clog << "[startEasyTask] AsyncTask delegated";
  at.wait();
  std::clog << "[startEasyTask] AsyncTask got result";
  auto retObj = at.getResult();
  return 0;
}

int AsyncTaskSamples::startLoopTask() {
  SampleTaskArgs a;
  a.id = 18;

  loop_worker_ = std::make_shared<AsyncTask>();
  std::clog << "[startLoopTask] AsyncTask constructed";
  loop_worker_->setWorkLoad(
      std::bind(&AsyncTaskSamples::loopTask, this, std::placeholders::_1, std::placeholders::_2));
  std::clog << "[startLoopTask] AsyncTask workload assigned";
  loop_worker_->runAsync(a);
  std::clog << "[startLoopTask] AsyncTask delegated";
  return 0;
}

int AsyncTaskSamples::endLoopTask() {
  for (int i = 0; i < 10; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::clog << "[endLoopTask] do simple work in caller thread...";
  }

  std::clog << "[endLoopTask] do large work in caller thread...";
  std::this_thread::sleep_for(std::chrono::seconds(10));

  loop_worker_->cancel();
  std::clog << "[endLoopTask] AsyncTask cancelled";
  loop_worker_->wait();
  std::clog << "[endLoopTask] AsyncTask wait for result";

  return 0;
}

std::shared_ptr<TaskResult> AsyncTaskSamples::easyTask(AsyncTask* t, TaskWorkArgs& e) {
  std::clog << "[easyTask %d] easyTask start!" << std::this_thread::get_id();

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  auto retObj = std::make_shared<TaskResult>();
  retObj->retVal = 1;
  retObj->desp = "done";

  std::clog << "[easyTask %d] easyTask end!" << std::this_thread::get_id();
  return retObj;
}

std::shared_ptr<TaskResult> AsyncTaskSamples::loopTask(AsyncTask* t, TaskWorkArgs& e) {
  std::clog << "[loopTask %d] loopTask start!" << std::this_thread::get_id();
  auto& arg = static_cast<SampleTaskArgs&>(e);

  int ret = arg.id;

  while (!t->isCancellationRequested()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::clog << "[loopTask %d] loopTask is working..." << std::this_thread::get_id();
    ret += 3;
    TaskProcess tp;
    tp.process = 10;
    t->reportProcess(tp);
  }

  auto retObj = std::make_shared<TaskResult>();
  retObj->retVal = ret;
  retObj->desp = "done";

  std::clog << "[loopTask %d] loopTask end! " << std::this_thread::get_id();
  return retObj;
}
