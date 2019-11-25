#include "Executor.h"


Executor::Executor()
	: m_taskQueue{std::make_shared<TaskQueue>()}
	, m_taskThread{std::unique_ptr<TaskThread>(new TaskThread(m_taskQueue))}
{
	m_taskThread->start();
}

Executor::~Executor() {
	shutdown();
}

void Executor::waitForSubmittedTasks() {
	std::promise<void> flushedPromise;
	auto flushedFuture = flushedPromise.get_future();
	auto task = [&flushedPromise]() {
		flushedPromise.set_value();
	};
	submit(task);
	flushedFuture.get();
}

void Executor::shutdown() {
	m_taskQueue->shutdown();
	m_taskThread.reset();
}

bool Executor::isShutdown() {
	return m_taskQueue->isShutdown();
}


