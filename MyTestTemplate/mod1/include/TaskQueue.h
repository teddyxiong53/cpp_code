#ifndef _TASK_QUEUE_H_
#define _TASK_QUEUE_H_

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <utility>


class TaskQueue {
public:
	TaskQueue();
	template <typename Task, typename... Args>
	auto push(Task task, Args&&... args) -> std::future<decltype(task(args...))>;
	
	template <typename Task, typename... Args>
	auto pushToFront(Task task, Args&&... args) -> std::future<decltype(task(args...))>;
	
	std::unique_ptr<std::function<void()>> pop();
	
	void shutdown();
	bool isShutdown();
	
private:
	using Queue = std::deque<std::unique_ptr<std::function<void ()>>>;
	template <typename Task, typename... Args>
	auto pushTo(bool front, Task task, Args&&... args) -> std::future<decltype(task(args...))>;
	
	Queue m_queue;
	std::condition_variable m_queueChanged;
	std::mutex m_queueMutex;
	std::atomic_bool m_shutdown;
	
};

template <typename Task, typename... Args>
auto TaskQueue::push(Task task, Args&&... args) -> std::future<decltype(task(args...))> {
	bool front = true;
	return pushTo(!front, std::forward<Task>(task), std::forward<Args>(args)...);
}

template <typename Task, typename... Args>
auto TaskQueue::pushToFront(Task task, Args&&... args) -> std::future<decltype(task(args...))> {
	bool front = true;
	return pushTo(front, std::forward<Task>(task), std::forward<Args>(args)...);
}

template <typename T>
inline static void forwardPromise(std::shared_ptr<std::promise<T>> promise, std::future<T>* future) {
	promise->set_value(future->get());
}

template <>
inline void forwardPromise(std::shared_ptr<std::promise<void>> promise, std::future<void>* future) {
	future->get();
	promise->set_value();
}

template <typename Task, typename... Args>
auto TaskQueue::pushTo(bool front, Task task, Args&&... args) 
	-> std::future<decltype(task(args...))> {
	auto boundTask = std::bind(std::forward<Task>(task), std::forward<Args>(args)...);
	using PackagedTaskType = std::packaged_task<decltype(boundTask())()>;
	auto packaged_task = std::make_shared<PackagedTaskType>(boundTask);
	
	auto cleanupPromise = std::make_shared<std::promise<decltype(task(args...))>>();
	auto cleanupFuture = cleanupPromise->get_future();
	
	auto translated_task = [packaged_task, cleanupPromise]() mutable {
		packaged_task->operator()();
		auto taskFuture = packaged_task->get_future();
		packaged_task.reset();
		forwardPromise(cleanupPromise, &taskFuture);
		
	};
	packaged_task.reset();
	{
		std::lock_guard<std::mutex> queueLock{m_queueMutex};
		if(!m_shutdown) {
			m_queue.emplace(front ? m_queue.begin() : m_queue.end(), new std::function<void()>(translated_task));
			
		} else {
			using FutureType = decltype(task(args...));
			return std::future<FutureType>();
		}
	}
	m_queueChanged.notify_all();
	return cleanupFuture;
}


#endif

