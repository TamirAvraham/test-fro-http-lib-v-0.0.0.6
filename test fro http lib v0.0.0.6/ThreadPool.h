#pragma once

#include <future>

#include <vector>

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>

#include <exception>

#include <memory>

#include "BlockingQueue.h"
#include "ThreadSafeQueue.h"

class ThreadPool
{
public:
	using WriteLock= std::unique_lock<std::shared_mutex>;
	using ReadLock= std::shared_lock<std::shared_mutex>;
	ThreadPool(int maxThreadNum);

	
	~ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator= (const ThreadPool&) = delete;
	ThreadPool& operator= (ThreadPool&&) = delete;

	//ctor an dtor type functions

	/// <summary>
	/// inits the thread pool
	/// </summary>
	/// <param name="amountOfThreads">the number of therads in the pool</param>
	void init(int amountOfThreads);
	/// <summary>
	/// stops the threadpool but process all delgeted tasks 
	/// </summary>
	void terminate();
	/// <summary>
	/// stops the threadpool and drop all tasks remained in queue
	/// </summary>
	void cancel();

	//observers

	bool inited() const ;
	bool isRunning()const;
	int size()const;


	// alloc aysnc task

	/// <summary>
	/// alloc task to thread pool
	/// </summary>
	/// <typeparam name="F"> function type</typeparam>
	/// <typeparam name="...Args">function args type</typeparam>
	/// <param name="f">function to alloc to a worker</param>
	/// <param name="...args">function args</param>
	/// <returns>future for function sent for alloc</returns>
	template<class F,class... Args> auto async(F&& f, Args&&... args) const->std::future<decltype(f(args...))>;

private:
	void spawn();

	bool _inited;
	bool _stop;
	bool _cancel;

	std::vector<std::thread> _workers;
	mutable ThreadSafeQueue<std::function<void()>> _tasks;

	mutable std::shared_mutex _mtx;
	mutable std::condition_variable_any _cond;
	mutable std::once_flag _one;
	

};

template<class F, class ...Args>
inline auto ThreadPool::async(F&& f, Args && ...args) const -> std::future<decltype(f(args ...))>
{
	using return_t = decltype(f(args ...));
	using future_t = std::future<return_t>;
	using task_t = std::packaged_task<return_t()>;
	//block for critical section
	{
		ReadLock lock(_mtx);
		if (_stop||_cancel)
		{
			throw std::runtime_error("Delegating task to a threadpool that has been terminated or canceled");
		}
	}
	auto bind_func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);//create function woht params
	std::shared_ptr<task_t> task = std::make_shared<task_t>(std::move(bind_func));//makte it into a shared ptr
	future_t fut = task->get_future();//get the future of it
	_tasks.emplace([task]()->void {(*task)(); });//add task to queue
	_cond.notify_one();//get 1 worker to do it
	return fut;// return fut so can be used in normal code
}
