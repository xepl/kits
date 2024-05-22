#include "../one/xepl.h"

#include <string>
#include <chrono>

namespace KITS::TIMER
{
	void Keyword_Time            ( XEPL::Cortex* _cortex );
	void Register_Performance_Kit( XEPL::Cortex* _cortex );
}

namespace KITS::TIMER
{
	class PerformanceTimer
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> start_clock;
		std::time_t start_time;
	public:
		~PerformanceTimer();
		PerformanceTimer( XEPL::Neuron*, XEPL::Gene* );
		
		std::string Time();
	};

	std::string formatMilliseconds( long long );
}
