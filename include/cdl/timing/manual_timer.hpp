// manual_timer.hpp
// Class Implementation: cdl::timing::manual_timer
//
// Simple timer class that is not controlled by its scope.

#include <chrono>

namespace cdl::timing
{
	template <typename DurationType>
	class manual_timer
	{
		std::chrono::time_point<
			std::chrono::high_resolution_clock
			> begin_;

	public:
		void start()
		{
			begin_ = std::chrono::high_resolution_clock::now();
		}

		DurationType stop()
		{
			auto end = std::chrono::high_resolution_clock::now();
			
			auto duration = end - begin_;
			return std::chrono::duration_cast<DurationType>(duration);
		}
	};

	using us_manual_timer  = manual_timer<std::chrono::microseconds>;
	using ms_manual_timer  = manual_timer<std::chrono::milliseconds>;
	using sec_manual_timer = manual_timer<std::chrono::seconds>;
}