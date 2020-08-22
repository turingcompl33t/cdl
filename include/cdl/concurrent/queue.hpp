// queue.hpp
//
// A minimal lock-based implementation of a thread-safe queue data structure.

#ifndef CDL_CONCURRENT_QUEUE_HPP
#define CDL_CONCURRENT_QUEUE_HPP

#include <queue>
#include <mutex>
#include <thread>
#include <utility>
#include <condition_variable>

namespace cdl::concurrent
{
	template <typename T>
	class queue
	{
		mutable std::mutex             lock;
		std::queue<std::shared_ptr<T>> buffer;
		std::condition_variable        nonempty;
	
	public:
		queue() = default;

		void wait_and_pop(T& value)
		{
			std::unique_lock<std::mutex> guard{lock};

			nonempty.wait(guard, [this] {return !buffer.empty(); });

			value = std::move(*buffer.front());
			buffer.pop();
		}

		std::shared_ptr<T> wait_and_pop()
		{
			std::unique_lock<std::mutex> guard{lock};

			nonempty.wait(guard, [this] {return !buffer.empty(); });

			auto item = buffer.front();
			buffer.pop();

			return item;
		}

		bool try_pop(T& value)
		{
			std::lock_guard<std::mutex> guard{lock};
			if (buffer.empty())
			{
				return false;
			}

			value = std::move(*buffer.front());
			buffer.pop();

			return true;
		}

		std::shared_ptr<T> try_pop()
		{
			std::lock_guard<std::mutex> guard{lock};
			if (buffer.empty())
			{
				return std::shared_ptr<T>{};
			}

			auto item = buffer.front();
			buffer.pop();

			return item;
		}

		void push(T new_item)
		{
			auto item = std::make_shared<T>(std::move(new_item));

			std::lock_guard<std::mutex> guard{lock};

			buffer.push(item);
			nonempty.notify_one();
		}

		bool is_empty_unsafe() const
		{
			std::lock_guard<std::mutex> guard{lock};
			return buffer.empty();
		}
	};
}

#endif // CDL_CONCURRENT_QUEUE_HPP