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

	if (message.find("[INFO]") != std::string::npos)
	{
		std::cout << "\033[33m";
	}

	if (message.find("[SHADER]") != std::string::npos)
	{
		std::cout << "\033[36m";
	}

	if (message.find("[SOUND]") != std::string::npos)
	{
		std::cout << "\033[35m";
	}

	if (message.find("[OPENGL]") != std::string::npos)
	{
		std::cout << "\033[96m";
	}

	if (message.find("]!") != std::string::npos)
	{
		std::cout << "\033[31m";
	}

	std::cout << std::fixed;

	std::cout << "[" << std::chrono::duration<float>(std::chrono::high_resolution_clock::now().time_since_epoch() - start_time).count() << "]" << message << "\n";

	std::cout << "\033[0m";
}
