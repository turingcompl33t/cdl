// thread_pool_waitable.hpp
// An extension of the simple thread pool
// (cdl::thread_pool_simple) implementation
// to support waitable work items.

#include <memory>
#include <future>
#include <utility>
#include <exception>
#include <type_traits>

#include "thread_joiner.hpp"
#include "concurrent_queue.hpp"

namespace cdl
{
	// function_wrapper
	//
	// A custom callable wrapper that handles move-only types.
	//
	// cdl::thread_pool_simple accepts work in the 
	// form of a std::function<> instance; however, this is no
	// longer possible in the event that std::packaged_task is
	// used to represent units of work because std::function 
	// requires that stored function objects are copy-constructible

	class function_wrapper
	{
		struct impl_base
		{
			virtual void call()  = 0;
			virtual ~impl_base() {}
		};

		std::unique_ptr<impl_base> impl_;

		template <typename FunctionType>
		struct impl_type : impl_base
		{
			FunctionType fn_;
			impl_type(FunctionType&& fn) : fn_{ std::move(fn) } {}
			void call() { fn_(); }
		};

	public:

		function_wrapper() = default;

		template <typename FunctionType>
		function_wrapper(FunctionType&& fn)
			: impl_{ std::make_unique<impl_type<FunctionType>>(std::move(fn)) }
		{}

		function_wrapper(function_wrapper&& other)
			: impl_{ std::move(other.impl_) }
		{}

		function_wrapper& operator=(function_wrapper&& rhs)
		{
			if (this == &rhs)
			{
				return *this;
			}

			impl_ = std::move(rhs.impl_);
			return *this;
		}

		function_wrapper(function_wrapper&) = delete;
		function_wrapper(function_wrapper const&) = delete;
		function_wrapper& operator=(function_wrapper&) = delete;

		void operator()()
		{
			impl_->call();
		}
	};

	// thread_pool_waitable
	//
	// Thread pool implementation wherein the submit()
	// method returns a future that can be waited on the
	// retrieve the results of tasks.

	class thread_pool_waitable
	{
		concurrent_queue<function_wrapper> work_queue_;

		std::atomic_bool         exit_;
		std::atomic_bool         accepting_;
		std::vector<std::thread> threads_;

		mutable std::mutex      empty_mutex_;
		std::condition_variable empty_cv_;

		thread_joiner joiner_;

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