#pragma once
#include <string>
#include <iostream>
class Logger
{
public:
	Logger();
	void log(const std::string& message);
	void exitWithError(const std::string& errorMessage);
	void SetOutput(std::ostream output);
private:
	
};

