// thread_joiner.hpp
// A simple thread-joiner class that ensures all
// joinable threads are joined prior to destruction.

#pragma once

#include <thread>
#include <vector>

namespace cdl
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

