// thread_pool_basic.hpp
// Class Implementation: cdl::threadpool::thread_pool_basic
// 
// Minimal thread pool implementation;
// does not support waitable tasks, thread-local work queues,
// or work stealing among worker threads.

#pragma once

#include <atomic>
#include <thread>
#include <vector>
#include <exception>
#include <functional>
#include <condition_variable>

#include "../concurrent/queue.hpp"
#include "../thread/thread_joiner.hpp"

namespace cdl::threadpool
{
	class thread_pool_basic
	{
	private:
		cdl::concurrent::queue<std::function<void()>> work_queue_;

		std::atomic_bool         exit_;
		std::atomic_bool         accepting_;
		std::vector<std::thread> threads_;
		
		mutable std::mutex      empty_mutex_;
		std::condition_variable empty_cv_;
		
		cdl::thread::thread_joiner joiner_;

	public:
		thread_pool_basic()
			: exit_{ false },
			accepting_{ true },
			joiner_{ threads_ }
		{
			unsigned const thread_count = std::thread::hardware_concurrency();

			try
			{
				for (unsigned i = 0; i < thread_count; ++i)
				{
					threads_.push_back(
						std::thread{ &thread_pool_basic::worker_thread, this }
					);
				}
			}
			catch (...)
			{
				exit_ = true;
				throw;
			}
		}

		~thread_pool_basic()
		{
			exit_ = true;
		}

		void shutdown()
		{
			std::unique_lock<std::mutex> lock{ empty_mutex_ };

			accepting_ = false;
			empty_cv_.wait(lock, [this] { return work_queue_.empty(); });
		}

		template <typename Function>
		void submit(Function work)
		{
			if (!accepting_)
			{
				throw std::runtime_error{ "Attempting Submit to Closed Thread Pool" };
			}

			work_queue_.push(std::function<void()>{work});
		}

	private:
		void worker_thread()
		{
			while (!exit_)
			{
				std::function<void()> task;
				if (work_queue_.try_pop(task))
				{
					task();
				}
				else
				{
					empty_cv_.notify_one();
					std::this_thread::yield();
				}
			}
		}
	};
}

