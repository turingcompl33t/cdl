// thread_guard.hpp
// Minimal RAII wrapper for thread objects which ensure the 
// thread is not joinable prior to destruction.

#include <thread>

#pragma once

namespace cdl
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