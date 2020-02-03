#pragma once

#include <string>
#include <Windows.h>

class Debugger
{

public:
	Debugger();
	~Debugger();

	void PrintLog(std::string content);
	void PrintLog(int content);
};

