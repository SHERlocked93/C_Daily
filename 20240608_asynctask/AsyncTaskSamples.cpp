#include "AsyncTaskSamples.h"

#include <chrono>
#include <iostream>

using namespace common_util::thread_task;

void AsyncTaskSamples::runSample() {
  startEasyTask();

  startLoopTask();
  endLoopTask();
}

int AsyncTaskSamples::startEasyTask() {
  SampleTaskArgs simpleTaskArgs_;
  simpleTaskArgs_.id = 18;

  AsyncTask task_;
  std::clog << "[startEasyTask] AsyncTask constructed" << std::endl;
  task_.setWorkLoad(std::bind(&AsyncTaskSamples::easyTask, this, std::placeholders::_1, std::placeholders::_2));
  std::clog << "[startEasyTask] AsyncTask workload assigned" << std::endl;
  task_.runAsync(simpleTaskArgs_);
  std::clog << "[startEasyTask] AsyncTask delegated" << std::endl;
  task_.wait();
  std::clog << "[startEasyTask] AsyncTask got result" << std::endl;
  auto retObj = task_.getResult();
  return 0;
}

int AsyncTaskSamples::startLoopTask() {
  SampleTaskArgs simpleTaskArgs_;
  simpleTaskArgs_.id = 18;

  loop_worker_ = std::make_shared<AsyncTask>();
  std::clog << "[startLoopTask] AsyncTask constructed" << std::endl;
  loop_worker_->setWorkLoad(
      std::bind(&AsyncTaskSamples::loopTask, this, std::placeholders::_1, std::placeholders::_2));
  std::clog << "[startLoopTask] AsyncTask workload assigned" << std::endl;
  loop_worker_->runAsync(simpleTaskArgs_);
  std::clog << "[startLoopTask] AsyncTask delegated" << std::endl;
  return 0;
}

int AsyncTaskSamples::endLoopTask() {
  for (int i = 0; i < 10; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::clog << "[endLoopTask] do simple work in caller thread..." << std::endl;
  }

  std::clog << "[endLoopTask] do large work in caller thread..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(10));

  loop_worker_->cancel();
  std::clog << "[endLoopTask] AsyncTask cancelled" << std::endl;
  loop_worker_->wait();
  std::clog << "[endLoopTask] AsyncTask wait for result" << std::endl;

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

  std::string ret = arg.id;

  while (!t->isCancellationRequested()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::clog << "[loopTask %d] loopTask is working..." << std::this_thread::get_id();
    ret += 3;
    TaskProcess tp;
    tp.process = 10;
    t->reportProcess(tp);
  }

  auto retObj = std::make_shared<TaskResult>();
  retObj->retVal = 0;
  retObj->desp = "done";

  std::clog << "[loopTask %d] loopTask end! " << std::this_thread::get_id();
  return retObj;
}
