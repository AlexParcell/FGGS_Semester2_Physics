#pragma once

#include <string>
#include <Windows.h>
#include "Vector.h"

class Debugger
{
	static Debugger* Instance;
public:
	Debugger();
	~Debugger();

	static Debugger* GetInstance()
	{
		if (Instance == nullptr)
		{
			Instance = new Debugger();
		}
		return Instance;
	}

	void PrintLog(std::string content);
	void PrintLog(int content);
	void PrintVector(std::string message, Vector v);
	void PrintFloat(float content);
};

