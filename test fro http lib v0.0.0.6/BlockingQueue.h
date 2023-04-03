#include <queue>
#include<shared_mutex>
template<class T>
class BlockingQueue :protected std::queue<T>
{
public:
	using WriteLock = std::unique_lock<std::shared_mutex>;
	using Readlock = std::shared_lock<std::shared_mutex>;


	BlockingQueue() {};//no need to impl

	~BlockingQueue() { clear(); }

	//remove all copy ctor's no need for them and defults

	//blocking_queue(const blocking_queue&) = delete;  
	//blocking_queue(blocking_queue&&) = delete;  
	//blocking_queue& operator=(const blocking_queue&) = delete; 
	//blocking_queue& operator=(blocking_queue&&) = delete;  



	///cheacks if the queue is empty
	inline bool empty()  const noexcept {
		Readlock lock(_mtx);
		return std::queue<T>::empty();
	}

	inline size_t size()  const noexcept {
		Readlock lock(_mtx);
		return std::queue<T>::size();
	}


	///clear the queue
	inline void clear() noexcept {
		WriteLock lock(_mtx);
		while (!empty())
		{
			std::queue<T>::pop();
		}
	}

	inline void push(const T& obj) noexcept {
		WriteLock lock(_mtx);
		std::queue<T>::push(obj);
	}

	template <typename... Args>
	inline void emplace(Args&&... args)noexcept {
		WriteLock lock(_mtx);
		std::queue<T>::emplace(std::forward<Args>(args)...);
	}

	inline bool pop(T& holder) {
		WriteLock lock(_mtx);
		if (empty())
		{
			return false;
		}
		holder = std::move(std::queue<T>::front());
		std::queue<T>::pop();
		return true;
	}

private:
	mutable std::shared_mutex _mtx;
};
