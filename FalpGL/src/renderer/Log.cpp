#include "Log.h"

void console_log(std::string message)
{
	static bool started = false;
	static std::chrono::steady_clock::duration start_time;

	if (!started)
	{
		start_time = std::chrono::high_resolution_clock::now().time_since_epoch();
		started = true;
	}

	std::cout << "[" << std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch() - start_time) << "]" << message << "\n";
}

