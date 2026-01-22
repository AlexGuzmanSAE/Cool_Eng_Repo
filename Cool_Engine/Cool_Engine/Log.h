#pragma once
#include <string>
#include <iostream>
class Log
{
public:

	static Log& instance()
	{
		static Log log;
		return log;
	}

	static void println(const std::string& str)
	{
		std::cout << str << std::endl;
	}

private:

	Log() = default;
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;
};

