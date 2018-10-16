#include "TaskThread.h"

TaskThread::TaskThread(std::shared_ptr<TaskQueue> taskQueue)
	: m_taskQueue{taskQueue}
	, m_shutdown{false}
{
	
}

TaskThread::~TaskThread() {
	m_shutdown = true;
	if(m_thread.joinable()) {
		m_thread.join();
	}
}


void TaskThread::start() {
	m_thread = std::thread{std::bind(&TaskThread::processTasksLoop, this)};
}

bool TaskThread::isShutdown() {
	return m_shutdown;
}

void TaskThread::processTasksLoop() {
	while(!m_shutdown) {
		auto m_actualTaskQueue = m_taskQueue.lock();
		if(m_actualTaskQueue && !m_actualTaskQueue->isShutdown()) {
			auto task = m_actualTaskQueue->pop();
			if(task) {
				task->operator()();
			}
		} else {
			m_shutdown = true;
		}
	}
}