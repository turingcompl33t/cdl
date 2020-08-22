// hierarchical_mutex.hpp
//
// Mutex wrapper classes that enforces strict lock hierarchy.
//
// Adapted from the implementation from 
// "C++ Concurrency in Action" by Anthony Williams.

#ifndef CDL_SYNC_HIERARCHICAL_MUTEX_HPP
#define CDL_SYNC_HIERARCHICAL_MUTEX_HPP

#include <mutex>
#include <exception>

namespace cdl::sync
{
	class hierarchical_mutex
	{
		std::mutex          mutex_;
		const unsigned long hierarchy_level_;
		unsigned long       previous_hierarchy_level_;

		// static 
		static thread_local unsigned long this_thread_hierarchy_level_;

	public:
		explicit hierarchical_mutex(const unsigned long level)
			: hierarchy_level_{ level },
			previous_hierarchy_level_{ 0 }
		{}

		void lock()
		{
			check_for_hierarchy_violation();
			mutex_.lock();
			update_hierarchy_level();
		}

		void unlock()
		{
			if (this_thread_hierarchy_level_ != hierarchy_level_)
			{
				throw std::logic_error{ "Mutex Hierarchy Violated" };
			}

			this_thread_hierarchy_level_ = previous_hierarchy_level_;
			mutex_.unlock();
		}

		bool try_lock()
		{
			check_for_hierarchy_violation();
			if (!mutex_.try_lock())
			{
				return false;
			}

			update_hierarchy_level();
			return true;
		}

	private:
		void check_for_hierarchy_violation()
		{
			if (this_thread_hierarchy_level_ <= hierarchy_level_)
			{
				throw std::logic_error{ "Mutex Hierarchy Violated" };
			}
		}

		void update_hierarchy_level()
		{
			previous_hierarchy_level_ = this_thread_hierarchy_level_;
			this_thread_hierarchy_level_ = hierarchy_level_;
		}
	};

	thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_level_{ ULONG_MAX };
}

#endif // CDL_SYNC_HIERARCHICAL_MUTEX_HPP