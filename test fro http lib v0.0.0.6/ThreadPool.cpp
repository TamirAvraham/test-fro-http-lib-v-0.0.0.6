#include "ThreadPool.h"

ThreadPool::ThreadPool(int maxThreadNum):_workers(),_tasks(),_mtx(), _one()
{
	_inited = _stop = _cancel = false;
	init(maxThreadNum);
}

ThreadPool::~ThreadPool()
{
	terminate();
}

void ThreadPool::init(int amountOfThreads)
{
	std::call_once(_one, [this, amountOfThreads]() {
		WriteLock lock(_mtx);
		_workers.reserve(amountOfThreads);
		for (int i = 0; i < amountOfThreads; i++)
			_workers.emplace_back(std::bind(&ThreadPool::spawn, this));
		_inited = true;
	});
}
void ThreadPool::terminate()
{
	{
		WriteLock lock(_mtx);
		if (isRunning()) {
			_stop = true;
		} else {
			return;
		}
	}
	_cond.notify_all();
	for (auto& worker : _workers) {
		worker.join();
	}


}
void ThreadPool::cancel()
{
	{
		WriteLock lock(_mtx);
		if (isRunning()) {
			_cancel = true;
		}
		else {
			return;
		}
	}
	_tasks.clear();
	_cond.notify_all();
	for (auto& worker : _workers) {
		worker.join();
	}
}
bool ThreadPool::inited() const
{
	ReadLock lock(_mtx);
	return _inited;
}
bool ThreadPool::isRunning() const
{
	ReadLock lock(_mtx);
	return _inited&&!_cancel&&!_stop;
}
int ThreadPool::size() const
{
	ReadLock lock(_mtx);
	return _workers.size();
}
/// <summary>
/// 
/// </summary>
void ThreadPool::spawn()
{
	for(;;)
	{
		bool pop = false;
		std::function<void()> task; 
		//temp scope for mtx
		{
			WriteLock lock(_mtx);

			_cond.wait(lock, [this, &pop, &task] {
				pop = _tasks.pop(task);
				return _cancel || _stop || pop;
			});

		}
		if (_cancel||(_stop&&!pop))
		{
			return;
		}
		task();
	}
}
