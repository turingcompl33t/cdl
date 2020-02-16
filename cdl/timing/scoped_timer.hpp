// scoped_timer.hpp
// Class Implementation: cdl::timing::scoped_timer
//
// Simple utility class that utilizes RAII to measure the
// execution time of a particular scope.

#include <chrono>
#include <ostream>

namespace cdl::timing
{
	template <typename DurationType>
	class scoped_timer
	{
		std::ostream& stream_;
		std::chrono::time_point<
			std::chrono::high_resolution_clock
			> begin_;

	public:
		scoped_timer(std::ostream& stream)
			: stream_{ stream }
		{
			begin_ = std::chrono::high_resolution_clock::now();
		}

		~scoped_timer()
		{
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = end - begin_;
			
			auto result = std::chrono::duration_cast<DurationType>(duration);
			stream_ << result.count();
		}
	};

	using us_scoped_timer  = scoped_timer<std::chrono::microseconds>;
	using ms_scoped_timer  = scoped_timer<std::chrono::milliseconds>;
	using sec_scoped_timer = scoped_timer<std::chrono::seconds>;
}