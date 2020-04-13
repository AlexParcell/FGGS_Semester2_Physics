#pragma once

#include <string>
#include <Windows.h>
#include "Vector.h"

class Debugger
{

public:
	Debugger();
	~Debugger();

	void PrintLog(std::string content);
	void PrintLog(int content);
	void PrintVector(std::string message, Vector v);
};

