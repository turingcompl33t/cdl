// scoped_thread.hpp
// Enhanced version of a basic thread RAII wrapper in which
// the wrapper class takes ownership of the underlying thread object.

#pragma once

#include <thread>
#include <utility>
#include <stdexcept>

namespace cdl
{
	class scoped_thread
	{
		std::thread t_;
	public:
		explicit scoped_thread(std::thread t)
			: t_{ std::move(t) }
		{
			if (!t_.joinable())
			{
				throw std::logic_error{ "Thread Not Joinable" };
			}
		}

		~scoped_thread()
		{
			t_.join();
		}

		scoped_thread(const scoped_thread&) = delete;
		scoped_thread& operator=(const scoped_thread&) = delete;
	};
}