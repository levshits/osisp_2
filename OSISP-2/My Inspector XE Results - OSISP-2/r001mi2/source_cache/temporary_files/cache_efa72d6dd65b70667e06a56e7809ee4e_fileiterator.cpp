#include "stdafx.h"
#include "stdio.h"
#include "windows.h"
#include "FileIterator.h"



FileIterator::FileIterator(wchar_t* path)
{
	directoryPath = (wchar_t*)malloc(wcslen(path) + 5);
	wcscpy(directoryPath,path);
}

FileIterator::~FileIterator()
{
	FindClose(hf);
}

wchar_t* FileIterator::NextFile()
{
	while (!isEnd)
	{

		if (hf == NULL)
		{
			wchar_t* buffer = (wchar_t*)malloc(wcslen(directoryPath) + 20);
			buffer = wcscpy(buffer, directoryPath);
			buffer = wcscat(buffer, L"\\*.*");
			hf = FindFirstFile(buffer, &FindFileData);
		}
		else
		{
			if (FindNextFile(hf, &FindFileData) == 0)
				isEnd = true;
		}
		if (!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
			break;
	}
	if (isEnd)
		return NULL;
	wchar_t* buffer = (wchar_t*)malloc((wcslen(directoryPath)+wcslen(FindFileData.cFileName) +3)*sizeof(wchar_t));
	wcscpy(buffer, directoryPath);
	wcscat(buffer, FindFileData.cFileName);
	wcscat(buffer, L"\0");
	return buffer;
}

bool FileIterator::IsNotEnd()
{
	return !isEnd;
}