// concurrent_queue.hpp
// A minimal threadsafe queue implementation.

#pragma once

#include <queue>
#include <mutex>
#include <thread>
#include <utility>
#include <condition_variable>

namespace cdl
{
	template <typename T>
	class concurrent_queue
	{
	private:
		mutable std::mutex             mutex_;
		std::queue<std::shared_ptr<T>> queue_;
		std::condition_variable        cv_;
	public:
		concurrent_queue() {};

		void wait_and_pop(T& value)
		{
			std::unique_lock<std::mutex> lock{ mutex_ };

			cv_.wait(lock, [this] {return !queue_.empty(); });

			value = std::move(*queue_.front());
			queue_.pop();
		}

		std::shared_ptr<T> wait_and_pop()
		{
			std::unique_lock<std::mutex> lock{ mutex_ };

			cv_.wait(lock, [this] {return !queue_.empty(); });

			auto item = queue_.front();
			queue_.pop();
			return item;
		}

		bool try_pop(T& value)
		{
			std::lock_guard<std::mutex> guard{ mutex_ };
			if (queue_.empty())
			{
				return false;
			}

			value = std::move(*queue_.front());
			queue_.pop();
			return true;
		}

		std::shared_ptr<T> try_pop()
		{
			std::lock_guard<std::mutex> guard{ mutex_ };
			if (queue_.empty())
			{
				return std::shared_ptr<T>{};
			}

			auto item = queue_.front();
			queue_.pop();
			return item;
		}

		void push(T new_item)
		{
			auto item = std::make_shared<T>(std::move(new_item));

			std::lock_guard<std::mutex> guard{ mutex_ };

			queue_.push(item);
			cv_.notify_one();
		}

		bool empty() const
		{
			std::lock_guard<std::mutex> guard{ mutex_ };
			return queue_.empty();
		}
	};
}

