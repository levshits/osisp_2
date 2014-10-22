// OSISP-2.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include <list>
#include "ThreadPool.h"
#include "FindHashOfFile.h"
#include "FileIterator.h"
#include <iostream>
#include <time.h>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	system("chcp 1251");
	system("cls");
	printf("Please enter size of pool\n",sizeof(wchar_t));
	int count;
	scanf("%d", &count);
	wchar_t* directory = static_cast<wchar_t*>(malloc(sizeof(wchar_t)* 100));
	printf("Please enter directory\n");
	fflush(stdin);
	wscanf(L"%[^\r\n]", directory);
	ThreadPool* pool = new ThreadPool(count);
	FileIterator* files = new FileIterator(directory);
	srand(time(nullptr));
	do
	{
		wchar_t* nextFile = files->NextFile();
		if (nextFile != NULL)
		{
			Task* task = new FindHashOfFile(nextFile);
			pool->Execute(task);
		}
	} while (files->IsNotEnd());
		
	delete pool;
	delete files;
	system("pause");
	return 0;
}

