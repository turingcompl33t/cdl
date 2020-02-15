// run_for.hpp
// Function Implementations: run_for(), run_for_periodic()
//
// Utility functions for executing functions for specified durations.

#include <chrono>
#include <future>
#include <functional>

namespace cdl::timing
{
	template <typename ChronoType>
	void run_for(
		ChronoType            duration,
		std::function<void()> task
	)
	{
		ChronoType total_elapsed{ 0 };

		while (total_elapsed < duration)
		{
			auto begin = std::chrono::high_resolution_clock::now();

			auto fut = std::async(std::launch::async, task);
			fut.wait();

			auto elapsed = std::chrono::high_resolution_clock::now() - begin;
			total_elapsed += std::chrono::duration_cast<ChronoType>(elapsed);
		}
	}

	template <typename DurationType, typename PeriodType>
	void run_for_periodic(
		DurationType          duration,
		PeriodType            period,
		std::function<void()> task
	)
	{
		DurationType total_elapsed{ 0 };

		auto begin = std::chrono::high_resolution_clock::now();

		auto fut = std::async(std::launch::async, task);
		fut.wait();

		auto elapsed = std::chrono::high_resolution_clock::now() - begin;
		total_elapsed += std::chrono::duration_cast<DurationType>(elapsed);

		while (total_elapsed < duration)
		{
			std::this_thread::sleep_for(period);

			auto loop_begin = std::chrono::high_resolution_clock::now();

			fut = std::async(std::launch::async, task);
			fut.wait();

			auto loop_elapsed = std::chrono::high_resolution_clock::now() - begin;
			total_elapsed += std::chrono::duration_cast<DurationType>(loop_elapsed);
		}
	}
}