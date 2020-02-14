// thread_pool_simple.hpp
// Minimal thread pool implementation;
// does not support waitable tasks, thread-local work queues,
// or work stealing among worker threads.

#pragma once

#include <atomic>
#include <thread>
#include <vector>
#include <functional>
#include <condition_variable>

#include "queue_mt.hpp"
#include "thread_joiner.hpp"

namespace cdl
{
	class thread_pool_simple
	{
	private:
		std::atomic_bool                exit_;
		queue_mt<std::function<void()>> work_queue_;
		std::vector<std::thread>        threads_;
		
		mutable std::mutex              empty_mutex_;
		std::condition_variable         empty_cv_;
		
		thread_joiner                   joiner_;

	public:
		thread_pool_simple()
			: exit_{ false },
			joiner_{ threads_ }
		{
			unsigned const thread_count = std::thread::hardware_concurrency();

			try
			{
				for (unsigned i = 0; i < thread_count; ++i)
				{
					threads_.push_back(
						std::thread{ &thread_pool_simple::worker_thread, this }
					);
				}
			}
			catch (...)
			{
				exit_ = true;
				throw;
			}
		}

		~thread_pool_simple()
		{
			exit_ = true;
		}

		void wait()
		{
			std::unique_lock<std::mutex> lock{ empty_mutex_ };
			empty_cv_.wait(lock, [this] { return work_queue_.empty(); });
		}

		template <typename Function>
		void submit(Function work)
		{
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

