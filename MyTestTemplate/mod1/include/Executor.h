#ifndef _EXECUTOR_H_
#define _EXECUTOR_H_

#include "TaskThread.h"
#include "TaskQueue.h"

class Executor {
public:
	Executor();
	~Executor();
	
	template <typename Task, typename... Args>
	auto submit(Task task, Args&&... args) -> std::future<decltype(task(args...))>;
	
	template <typename Task, typename... Args>
	auto submitToFront(Task task, Args&&... args) -> std::future<decltype(task(args...))>;
	
	void waitForSubmittedTasks();
	void shutdown();
	bool isShutdown();
	

private:
	std::shared_ptr<TaskQueue> m_taskQueue;
	std::unique_ptr<TaskThread> m_taskThread;
};

template <typename Task, typename... Args>
auto Executor::submit(Task task, Args&&... args) -> std::future<decltype(task(args...))> {
	return m_taskQueue->push(task, std::forward<Args>(args)...);
	
}
template <typename Task, typename... Args>
auto Executor::submitToFront(Task task, Args&&... args) -> std::future<decltype(task(args...))> {
	return m_taskQueue->pushToFront(task, std::forward<Args>(args)...);
	
}
#endif

