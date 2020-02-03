#include "Debugger.h"

Debugger::Debugger()
{

}

Debugger::~Debugger()
{

}

void Debugger::PrintLog(std::string content)
{
	content.append("\n");
	OutputDebugStringA(content.c_str());
}

void Debugger::PrintLog(int content)
{
	char buffer[1024] = { 0 };
	sprintf_s(buffer, "%d \n", content);
	OutputDebugStringA(buffer);
}