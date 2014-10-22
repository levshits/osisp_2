#pragma once
#include "Task.h"
class FindHashOfFile :
	public Task
{
public:
	FindHashOfFile(wchar_t* filename);
	~FindHashOfFile();
	virtual void Run() override;
	virtual void Log() override;
private:
	wchar_t* logmessage = nullptr;
	wchar_t* filename;
};

