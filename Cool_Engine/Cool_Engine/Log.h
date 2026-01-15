#pragma once
#include <string>
#include <iostream>
class Log
{
public:
	static void println(const char* str)
	{
		std::cout << str << std::endl;
	}
};

