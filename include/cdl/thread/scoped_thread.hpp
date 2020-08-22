// scoped_thread.hpp
//
// Enhanced version of a basic thread RAII wrapper in which
// the wrapper class takes ownership of the underlying thread object.

#ifndef CDL_THREAD_SCOPED_THREAD_HPP
#define CDL_THREAD_SCOPED_THREAD_HPP

#include <thread>
#include <utility>
#include <stdexcept>

namespace cdl::thread
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

		scoped_thread(const scoped_thread&)            = delete;
		scoped_thread& operator=(const scoped_thread&) = delete;
	};
}

#endif // CDL_THREAD_SCOPED_THREAD_HPP