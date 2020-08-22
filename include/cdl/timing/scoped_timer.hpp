// scoped_timer.hpp
//
// Simple utility class that utilizes RAII to 
// measure the execution time of a particular scope.

#ifndef CDL_TIMING_SCOPED_TIMER_HPP
#define CDL_TIMING_SCOPED_TIMER_HPP

#include <chrono>
#include <cstdint>

namespace cdl::timing
{
	template <typename DurationType>
	class scoped_timer
	{
		std::uint64_t& out;

		std::chrono::time_point<
			std::chrono::high_resolution_clock
			> begin;

	public:
		explicit scoped_timer(std::uint64_t& out_)
			: out{out_}
		{
			begin = std::chrono::high_resolution_clock::now();
		}

		~scoped_timer()
		{
			auto end = std::chrono::high_resolution_clock::now();			
			auto elapsed = std::chrono::duration_cast<DurationType>(end - begin);
			out = elapsed.count();
		}
	};

	using us_scoped_timer  = scoped_timer<std::chrono::microseconds>;
	using ms_scoped_timer  = scoped_timer<std::chrono::milliseconds>;
	using sec_scoped_timer = scoped_timer<std::chrono::seconds>;
}

#endif // CDL_TIMING_SCOPED_TIMER_HPP