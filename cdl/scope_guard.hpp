// scope_guard.hpp
// Declarative control flow utilites.

#pragma once

#include <utility>
#include <exception>
#include <functional>

namespace cdl
{
	namespace detail
	{

		// uncaught_exception_counter
		//
		// Simple helper class that uses c++17
		// std::uncaught_exceptions() to determine if 
		// the enclosing scope generated a new exception

		class uncaught_exception_counter
		{
			int exception_count_;
		public:
			uncaught_exception_counter()
				: exception_count_(std::uncaught_exceptions())
			{}

			bool new_uncaught_exception() noexcept
			{
				return std::uncaught_exceptions() > exception_count_;
			}
		};

		// scope_guard
		//
		// General scope guard implementation utilized
		// in SCOPE_EXIT macro to execute the provided
		// function upon exit from enclosing scope

		template <typename Func>
		class scope_guard
		{
		public:
			scope_guard(const Func& cleanup)
				: cleanup_{ cleanup }
			{}

			scope_guard(scope_guard&& other)
				: cleanup_{ std::move(other.cleanup_) }
			{
				dismiss(other);
			}

			~scope_guard()
			{
				cleanup_();
			}

			scope_guard(const scope_guard& other) = delete;
			scope_guard& operator=(const scope_guard& rhs) = delete;

			friend
				void dismiss(scope_guard& g)
			{
				g.cleanup_ = [] {};
			}

		private:
			std::function<void()> cleanup_;
		};

		// scope_guard_for_new_exception
		//
		// Scope guard implementation utilize by 
		// both SCOPE_SUCCESS and SCOPE_FAIL to execute
		// the provided function in the event that the 
		// enclosing scope is exited under success or 
		// failure conditions, respectively

		template <typename Func, bool execute>
		class scope_guard_for_new_exception
		{
			Func function_;
			uncaught_exception_counter counter_;
		public:
			explicit scope_guard_for_new_exception(const Func& fn)
				: function_{ fn }
			{}

			explicit scope_guard_for_new_exception(Func&& fn)
				: function_{ std::move(fn) }
			{}

			~scope_guard_for_new_exception() noexcept(execute)
			{
				if (execute == counter_.new_uncaught_exception())
				{
					function_();
				}
			}
		};

		// scope_guard_on_exit
		//
		// Execute the provided function upon scope exit

		enum class scope_guard_on_exit {};

		template <typename Func>
		scope_guard<Func>
		operator+(scope_guard_on_exit, Func&& fn)
		{
			return scope_guard<Func>(std::forward<Func>(fn));
		}

		// scope_guard_on_fail
		//
		// Execute the provided function upon exceptional
		// exit from enclosing scope

		enum class scope_guard_on_fail {};

		template <typename Func>
		scope_guard_for_new_exception<
			typename std::decay<Func>::type, true>
		operator+(scope_guard_on_fail, Func&& fn)
		{
			return scope_guard_for_new_exception<
				typename std::decay<Func>::type, true>
			{
				std::forward<Func>(fn)
			};
		}

		// scope_guard_on_success
		//
		// Execute the provided function upon non-exceptional
		// exit from enclosing scope

		enum class scope_guard_on_success {};

		template <typename Func>
		scope_guard_for_new_exception<
			typename std::decay<Func>::type, false>
		operator+(scope_guard_on_success, Func&& fn)
		{
			return scope_guard_for_new_exception<
				typename std::decay<Func>::type, false>
			{
				std::forward<Func>(fn)
			};
		}
	}
}

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)
#endif

#define SCOPE_EXIT \
	auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) \
	= cdl::detail::scope_guard_on_exit{} + [&]()

#define SCOPE_FAIL \
	auto ANONYMOUS_VARIABLE(SCOPE_FAIL_STATE) \
	= cdl::detail::scope_guard_on_fail{} + [&]() noexcept

#define SCOPE_SUCCESS \
	auto ANONYMOUS_VARIABLE(SCOPE_SUCCESS_STATE) \
	= cdl::detail::scope_guard_on_success{} + [&]() noexcept