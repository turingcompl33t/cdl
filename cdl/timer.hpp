// timer.hpp
// Utility class for generating random wait times from distributions.

#include <atomic>
#include <random>
#include <chrono>
#include <thread>
#include <future>
#include <functional>

namespace cdl
{
	template <typename ChronoType>
	class exponential_wait
	{
	private:
		std::random_device                    device_;
		std::mt19937                          generator_;
		std::exponential_distribution<double> wait_distribution_;
	public:
		explicit exponential_wait(double mean_dwell)
			: device_{}, generator_{ device_() },
			wait_distribution_ {1. / mean_dwell }
		{}

		ChronoType next()
		{
			auto const wait_double = wait_distribution_( generator_ );
			auto const wait_chrono = ChronoType
			{
				static_cast<unsigned long long>(wait_double) 
			};

			return wait_chrono;
		}
	};

	template <typename ChronoType, typename IntType>
	class uniform_wait
	{
		std::random_device                     device_;
		std::mt19937                           generator_;
		std::uniform_int_distribution<IntType> wait_distribution_;
	public:
		explicit uniform_wait(IntType low, IntType high)
			: device_{}, generator_{device_()},
			wait_distribution_{low, high}
		{}

		ChronoType next()
		{
			auto next_wait  = wait_distribution_(generator_);
			return ChronoType{ next_wait };
		}
	};

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