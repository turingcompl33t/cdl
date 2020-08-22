// thread_guard.hpp
//
// Minimal RAII wrapper for thread objects which ensures
// the associated thread is not joinable prior to destruction.

#ifndef CDL_THREAD_THREAD_GUARD
#define CDL_THREAD_THREAD_GUARD

#include <thread>

namespace cdl::thread
{
	class thread_guard
	{
		std::thread& t_;
	public:
		explicit thread_guard(std::thread& t)
			: t_{ t }
		{}

		~thread_guard()
		{
			if (t_.joinable())
			{
				t_.join();
			}
		}
	};
}

#endif // CDL_THREAD_THREAD_GUARD