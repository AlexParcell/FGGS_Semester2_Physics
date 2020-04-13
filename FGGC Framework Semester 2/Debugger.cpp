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

void Debugger::PrintVector(std::string message, Vector v)
{
	OutputDebugStringA(message.c_str());
	
	char buffer[1024] = { 0 };
	sprintf_s(buffer, "%f, %f, %f \n", v.X, v.Y, v.Z);
	OutputDebugStringA(buffer);
}