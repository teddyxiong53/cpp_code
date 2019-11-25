#include "TaskQueue.h"

TaskQueue::TaskQueue()
	: m_shutdown{false}
{
	
}

void TaskQueue::shutdown() {
	std::lock_guard<std::mutex> queueLock{m_queueMutex};
	m_queue.clear();
	m_shutdown = true;
	m_queueChanged.notify_all();
}

bool TaskQueue::isShutdown() {
	return m_shutdown;
}

std::unique_ptr<std::function<void()>> TaskQueue::pop() {
	std::unique_lock<std::mutex> queueLock{m_queueMutex};
	auto shouldNotWait = [this]() {
		return m_shutdown || !m_queue.empty();
	};
	if(!shouldNotWait()) {
		m_queueChanged.wait(queueLock, shouldNotWait);
	}
	if(!m_queue.empty()) {
		auto task = std::move(m_queue.front());
		m_queue.pop_front();
		return task;
	}
	return nullptr;
}

