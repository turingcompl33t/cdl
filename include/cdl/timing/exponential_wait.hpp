// exponential_wait.hpp
//
// Helper class for generating exponentially-distributed random durations.

#ifndef CDL_TIMING_EXPONENTIAL_WAIT_HPP
#define CDL_TIMING_EXPONENTIAL_WAIT_HPP

#include <random>
#include <chrono>

namespace cdl::timing
{
	template <typename ChronoType>
	class exponential_wait
	{
	private:
		std::random_device                    device;
		std::mt19937                          generator;
		std::exponential_distribution<double> wait_distribution;
	public:
		explicit exponential_wait(double mean_dwell)
			: device{}, generator{ device() },
			wait_distribution{ 1. / mean_dwell }
		{}

		ChronoType next()
		{
			auto const wait_double = wait_distribution(generator);
			auto const wait_chrono = ChronoType
			{
				static_cast<unsigned long long>(wait_double)
			};

			return wait_chrono;
		}
	};

	using exponential_us_wait  = exponential_wait<std::chrono::microseconds, unsigned>;
	using exponential_ms_wait  = exponential_wait<std::chrono::milliseconds, unsigned>;
	using exponential_sec_wait = exponential_wait<std::chrono::seconds, unsigned>;
}

#endif // CDL_TIMING_EXPONENTIAL_WAIT_HPP