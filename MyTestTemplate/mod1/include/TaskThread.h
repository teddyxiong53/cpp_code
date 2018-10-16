#ifndef _TASK_THREAD_H_
#define _TASK_THREAD_H_
#include <mutex>
#include <atomic>
#include <memory>
#include <thread>


#include "TaskQueue.h"

class TaskThread {
public:
	TaskThread(std::shared_ptr<TaskQueue> taskQueue);
	~TaskThread();
	void start();
	bool isShutdown();
private:
	void processTasksLoop();
	std::weak_ptr<TaskQueue> m_taskQueue;
	std::atomic_bool m_shutdown;
	std::thread m_thread;
};
#endif

