// thread_joiner.hpp
// Class Implementation: cdl::thread::thread_joiner
//
// A simple thread-joiner class that ensures all
// joinable threads are joined prior to destruction.

#pragma once

#include <thread>
#include <vector>

namespace cdl::thread
{
	class thread_joiner
	{
		std::vector<std::thread>& threads_;

	public:
		explicit thread_joiner(std::vector<std::thread>& threads)
			: threads_{ threads }
		{}

		~thread_joiner()
		{
			wait_all();
		}

		void wait_all()
		{
			for (auto& t : threads_)
			{
				if (t.joinable())
				{
					t.join();
				}
			}
		}
	};
}

