#pragma once
#include "Windows.h"
class FileIterator
{
public:
	FileIterator(wchar_t *path);
	~FileIterator();
	wchar_t* NextFile();
	bool IsNotEnd();

private:
	HANDLE hf = NULL;
	bool isEnd = false;
	WIN32_FIND_DATA FindFileData;
	wchar_t* directoryPath;
};

