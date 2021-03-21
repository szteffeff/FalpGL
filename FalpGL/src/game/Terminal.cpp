#include "Terminal.h"

void out::warn(std::string message)
{
	std::cout << "[WARN]: " << message << std::endl;
}

void out::error(std::string message)
{
	std::cout << "[ERROR]: " << message << std::endl;
}

void out::info(std::string message)
{
	std::cout << "[INFO]: " << message << std::endl;
}
