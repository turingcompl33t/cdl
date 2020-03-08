// stack.hpp
// Class Implementation: cdl::concurrent::stack
// A minimal lock-based implementation of a thread-safe stack data structure.

#include <stack>
#include <mutex>
#include <exception>

namespace cdl::concurrent
{
	struct empty_stack_exception : std::exception 
	{
		//const char* what() const noexcept;
	};

	template <typename T>
	class stack
	{
		std::stack<T>      stack_;
		mutable std::mutex mutex_;
	public:
		stack() {}

		stack(stack const& other)
		{
			std::lock_guard<std::mutex> guard{ other.mutex_ };
			stack_ = other.stack_;
		}
		stack& operator=(stack const&) = delete;

		void push(T new_item)
		{
			std::lock_guard<std::mutex> guard{ mutex_ };
			stack_.push(std::move(new_item));
		}

		std::shared_ptr<T> pop()
		{
			std::lock_guard<std::mutex> guard{ mutex_ };
			if (stack_.empty())
			{
				throw empty_stack_exception{};
			}

			auto const item = std::make_shared<T>(std::move(stack_.top()));
			stack_.pop();

			return item;
		}

		void pop(T& item)
		{
			std::lock_guard<std::mutex> guard{ mutex_ };
			if (stack_.empty())
			{
				throw empty_stack_exception{};
			}

			item = std::move(stack_.top());
			stack_.pop();
		}

		bool empty() const
		{
			std::lock_guard<std::mutex> guard{ mutex_ };
			return stack_.empty();
		}
	};
}