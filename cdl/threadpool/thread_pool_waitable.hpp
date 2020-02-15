// thread_pool_waitable.hpp
// Class Implementation: cdl::threadpool::thread_pool_waitable
//
// An extension of the simple thread pool
// (cdl::threadpool::thread_pool_basic) implementation
// to support waitable work items.

#include <mutex>
#include <memory>
#include <future>
#include <utility>
#include <exception>
#include <type_traits>
#include <condition_variable>

#include "function_wrapper.hpp"
#include "../concurrent/queue.hpp"
#include "../thread/thread_joiner.hpp"

namespace cdl::threadpool
{
	// thread_pool_waitable
	//
	// Thread pool implementation wherein the submit()
	// method returns a future that can be waited on the
	// retrieve the results of tasks.

	class thread_pool_waitable
	{
		cdl::concurrent::queue<function_wrapper> work_queue_;

		std::atomic_bool         exit_;
		std::atomic_bool         accepting_;
		std::vector<std::thread> threads_;

		mutable std::mutex      empty_mutex_;
		std::condition_variable empty_cv_;

		cdl::thread::thread_joiner joiner_;

	public:
		thread_pool_waitable()
			: accepting_{ true },
			exit_{ false },
			joiner_{ threads_ }
		{
			unsigned const thread_count = std::thread::hardware_concurrency();

			try
			{
				for (unsigned i = 0; i < thread_count; ++i)
				{
					threads_.push_back(
						std::thread{ &thread_pool_waitable::worker_thread, this }
					);
				}
			}
			catch (...)
			{
				exit_ = true;
				throw;
			}
		}

		~thread_pool_waitable()
		{
			exit_ = true;
		}

		void shutdown()
		{
			std::unique_lock<std::mutex> lock{ empty_mutex_ };
			
			accepting_ = false;
			empty_cv_.wait(lock, [this] { return work_queue_.empty(); });
		}

		template <typename FunctionType>
		std::future<typename std::result_of_t<FunctionType()>>
		submit(FunctionType fn)
		{
			using result_type = typename std::result_of_t<FunctionType()>;

			if (!accepting_)
			{
				throw std::runtime_error{ "Attempting Submit to Closed Thread Pool" };
			}
			
			std::packaged_task<result_type()> task{ std::move(fn) };
			
			auto fut = task.get_future();
			work_queue_.push(std::move(task));

			return fut;
		}

	private:
		void worker_thread()
		{
			while (!exit_)
			{
				function_wrapper task;
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