// jthread.hpp
//
// Minimal implementation of (forthcoming in c++20) std::jthread
// which resembles a std::thread object with the additional semantics
// that it automatically joins in its destructor.

#ifndef CDL_THREAD_JTHREAD_HPP
#define CDL_THREAD_JTHREAD_HPP

#include <thread>
#include <utility>

namespace cdl::thread
{
	class jthread
	{
		std::thread t_;

	public:
		jthread() noexcept = default;

		template <typename Callable, typename ... Args>
		explicit jthread(Callable&& func, Args&&... args)
			: t_{ std::forward<Callable>(func), std::forward<Args>(args)... }
		{}

		explicit jthread(std::thread t) noexcept
			: t_{ std::move(t) }
		{}

		jthread(jthread&& other) noexcept
			: t_{ std::move(other.t_) }
		{}

		jthread& operator=(jthread&& rhs) noexcept
		{
			if (joinable())
			{
				join();
			}

			t_ = std::move(rhs.t_);
			return *this;
		}

		jthread& operator=(std::thread rhs) noexcept
		{
			if (joinable())
			{
				join();
			}

			t_ = std::move(rhs);
			return *this;
		}

		~jthread() noexcept
		{
			if (joinable())
			{
				join();
			}
		}

		void swap(jthread& other) noexcept
		{
			t_.swap(other.t_);
		}

		std::thread::id get_id() const noexcept
		{
			return t_.get_id();
		}

		bool joinable()
		{
			return t_.joinable();
		}

		void join()
		{
			t_.join();
		}

		void detach()
		{
			t_.detach();
		}

		std::thread& as_thread() noexcept
		{
			return t_;
		}

		const std::thread& as_thread() const noexcept
		{
			return t_;
		}
	};
}

#endif // CDL_THREAD_JTHREAD_HPP