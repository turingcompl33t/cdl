// uniform_wait.hpp
//
// Helper class for generating uniformly-distributed random durations.

#ifndef CDL_TIMING_UNIFORM_WAIT_HPP
#define CDL_TIMING_UNIFORM_WAIT_HPP

#include <random>
#include <chrono>

namespace cdl::timing
{
	template <typename ChronoType, typename IntType>
	class uniform_wait
	{
		std::random_device                     device;
		std::mt19937                           generator;
		std::uniform_int_distribution<IntType> wait_distribution;
	public:
		explicit uniform_wait(IntType low, IntType high)
			: device{}, generator{ device() },
			wait_distribution{ low, high }
		{}

		ChronoType next()
		{
			auto next_wait = wait_distribution(generator);
			return ChronoType{ next_wait };
		}
	};

	using uniform_us_wait  = uniform_wait<std::chrono::microseconds, unsigned>;
	using uniform_ms_wait  = uniform_wait<std::chrono::milliseconds, unsigned>;
	using uniform_sec_wait = uniform_wait<std::chrono::seconds, unsigned>;
}

#endif // CDL_TIMING_UNIFORM_WAIT_HPP