// joining_thread.hpp
// Class Implementation: cdl::thread::joining_thread
//
// Minimal implementation of (possibly forthcoming) std::jthread
// which resembles a std::thread object with the additional semantics
// that it automatically joins in its destructor.

#pragma once

#include <thread>
#include <utility>

namespace cdl::thread
{
	class joining_thread
	{
		std::thread t_;

	public:
		joining_thread() noexcept = default;

		template <typename Callable, typename ... Args>
		explicit joining_thread(Callable&& func, Args&&... args)
			: t_{ std::forward<Callable>(func), std::forward<Args>(args)... }
		{}

		explicit joining_thread(std::thread t) noexcept
			: t_{ std::move(t) }
		{}

		joining_thread(joining_thread&& other) noexcept
			: t_{ std::move(other.t_) }
		{}

		joining_thread& operator=(joining_thread&& rhs) noexcept
		{
			if (joinable())
			{
				join();
			}

			t_ = std::move(rhs.t_);
			return *this;
		}

		joining_thread operator=(std::thread rhs) noexcept
		{
			if (joinable())
			{
				join()
			}

			t_ = std::move(rhs);
			return *this;
		}

		~joining_thread() noexcept
		{
			if (joinable())
			{
				join();
			}
		}

		void swap(joining_thread& other) noexcept
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
	}
}