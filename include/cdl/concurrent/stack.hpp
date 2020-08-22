// stack.hpp
// A minimal lock-based implementation of a thread-safe stack data structure.

#ifndef CDL_CONCURRENT_STACK_HPP
#define CDL_CONCURRENT_STACK_HPP

#include <stack>
#include <mutex>
#include <exception>

namespace cdl::concurrent
{
	struct empty_stack_exception 
		: std::exception {};

	template <typename T>
	class stack
	{
		std::stack<T>      buffer;
		mutable std::mutex lock;

	public:
		stack() = default;

		void push(T new_item)
		{
			std::lock_guard<std::mutex> guard{lock};
			buffer.push(std::move(new_item));
		}

		std::shared_ptr<T> pop()
		{
			std::lock_guard<std::mutex> guard{lock};
			if (buffer.empty())
			{
				throw empty_stack_exception{};
			}

			auto const item = std::make_shared<T>(std::move(buffer.top()));
			buffer.pop();

			return item;
		}

		void pop(T& item)
		{
			std::lock_guard<std::mutex> guard{lock};
			if (buffer.empty())
			{
				throw empty_stack_exception{};
			}

			item = std::move(buffer.top());
			buffer.pop();
		}

		bool is_empty_unsafe() const
		{
			std::lock_guard<std::mutex> guard{lock};
			return buffer.empty();
		}
	};
}

#endif // CDL_CONCURRENT_STACK_HPP