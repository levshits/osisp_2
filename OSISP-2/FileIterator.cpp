#include "stdafx.h"
#include "stdio.h"
#include "windows.h"
#include "FileIterator.h"



FileIterator::FileIterator(wchar_t* path)
{
	directoryPath = path;
}

FileIterator::~FileIterator()
{
	if (directoryPath != NULL)
		free(directoryPath);
	FindClose(hf);
}

wchar_t* FileIterator::NextFile()
{
	while (!isEnd)
	{

		if (hf == NULL)
		{
			wchar_t* buffer = static_cast<wchar_t*>(malloc(wcslen(directoryPath) + 20));
			buffer = wcscpy(buffer, directoryPath);
			buffer = wcscat(buffer, L"\\*.*");
			hf = FindFirstFile(buffer, &FindFileData);
			free(buffer);
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
		return nullptr;
	wchar_t* buffer = static_cast<wchar_t*>(malloc((wcslen(directoryPath)+wcslen(FindFileData.cFileName) +3)*sizeof(wchar_t)));
	wcscpy(buffer, directoryPath);
	wcscat(buffer, FindFileData.cFileName);
	wcscat(buffer, L"\0");
	return buffer;
}

bool FileIterator::IsNotEnd()
{
	return !isEnd;
}