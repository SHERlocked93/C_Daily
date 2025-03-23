#pragma once
// �Ľ�����
// 1. �ϲ��򵥶�ʱ����һ���߳���
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

  int interval_millseconds_{0};  // ������ʱ�� 0��ʾ�������� ms
};

class TaskResultBase {
 public:
  int retVal_ = -1;   // 0�ɹ� -2��ʱ ����ʧ��
  std::string desp_;  // �������
  virtual ~TaskResultBase() = default;
};

class TaskProcessBase {
 public:
  int process_ = 0;      // 0��ʼ ���ڵ���100���
  std::string message_;  // ������Ϣ
  virtual ~TaskProcessBase() = default;
};

// ����ȡ������
enum class TaskCancelType : int {
  TIMEOUT,  // ��ʱȡ��
  MANUAL,   // �ֶ�ȡ��
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

  //! ��������
  EasyAsyncTask* setWorkLoad(const TaskWorkLoad& workLoad);

  //! ����������ɻص�
  EasyAsyncTask* setTaskDoneHandler(const TaskDoneHandler& dHandler);
  //! ����������Ȼص�
  EasyAsyncTask* setProcessHandler(const TaskProcessHandler& pHandler);
  //! ��������ȡ���ص�
  EasyAsyncTask* setCancelHandler(const TaskCancelHandler& pHandler);

  //! �����첽����
  //! \param args ʱ����
  EasyAsyncTask* runAsync(const std::shared_ptr<TaskWorkArgsBase>& args);
  //! ���ó�ʱʱ�� ��λms
  EasyAsyncTask* setTimeoutDuration(int timeout_duration_ms_);

  int cancel();
  void wait() const;

  //! ��ȡ������
  std::shared_ptr<TaskResultBase> getResult();
  //! �ϱ��������
  void reportProcess(TaskProcessBase& process) const;
  //! ��ȡ�Ƿ�ȡ��
  bool isCancellationRequested() const;

 private:
  TaskWorkLoad task_work_load_ = nullptr;            // ����
  TaskProcessHandler process_reporter_ = nullptr;    // ���Ȼص�
  TaskDoneHandler task_done_handler_ = nullptr;      // ��ɻص�
  TaskCancelHandler task_cancel_handler_ = nullptr;  // ȡ���ص���ֻѭ��������

  bool is_work_load_bind_ = false;            // �Ƿ������
  bool is_process_reporter_bind_ = false;     // �Ƿ�󶨽��Ȼص�
  bool is_task_done_handler_bind_ = false;    // �Ƿ����ɻص�
  bool is_task_cancel_handler_bind_ = false;  // �Ƿ��ȡ���ص�

  std::shared_ptr<TaskResultBase> result_obj_;  // ������
  std::shared_ptr<std::thread> thread_;         // �����߳�
  std::shared_ptr<TaskWorkArgsBase> args_{};    // �洢�������

  //! �������߳�
  void privateRunner(EasyAsyncTask* t,
                     const std::shared_ptr<TaskWorkArgsBase>& args);

  // std::condition_variable cv_;
  std::mutex mutex_;
  std::atomic<int> cancel_pending_{0};
  std::atomic<int> is_working_{0};
  std::atomic<bool> is_waiting_{false};
  std::chrono::time_point<std::chrono::system_clock> start_time_;  // ��ʼʱ��
  std::chrono::milliseconds timeout_duration_;  // ��ʱʱ�� ms 0Ϊ����ʱ

  const std::chrono::milliseconds timeout_check_duration_ =
      std::chrono::milliseconds(20);  // ��ʱ����� ms
};
}  // namespace thread_task
