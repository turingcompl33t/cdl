// exponential_wait.hpp
// Class Implementation: cdl::timing::exponential_wait
//
// Helper class for generating exponentially-distributed random durations.

#include <random>
#include <chrono>

namespace cdl::timing
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
			wait_distribution_{ 1. / mean_dwell }
		{}

		ChronoType next()
		{
			auto const wait_double = wait_distribution_(generator_);
			auto const wait_chrono = ChronoType
			{
				static_cast<unsigned long long>(wait_double)
			};

			return wait_chrono;
		}
	};
}