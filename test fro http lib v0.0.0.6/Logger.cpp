#include "Logger.h"



Logger::Logger()
{
}

void Logger::log(const std::string& message)
{
	std::cout << message << '\n';
}

void Logger::exitWithError(const std::string& errorMessage)
{
	log("ERROR: " + errorMessage);
	exit(1);
}
