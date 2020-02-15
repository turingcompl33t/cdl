// uniform_wait.hpp
// Class Implementation: cdl::timing::uniform_wait
//
// Helper class for generating uniformly-distributed random durations.

#include <random>
#include <chrono>

namespace cdl::timing
{
	template <typename ChronoType, typename IntType>
	class uniform_wait
	{
		std::random_device                     device_;
		std::mt19937                           generator_;
		std::uniform_int_distribution<IntType> wait_distribution_;
	public:
		explicit uniform_wait(IntType low, IntType high)
			: device_{}, generator_{ device_() },
			wait_distribution_{ low, high }
		{}

		ChronoType next()
		{
			auto next_wait = wait_distribution_(generator_);
			return ChronoType{ next_wait };
		}
	};
}