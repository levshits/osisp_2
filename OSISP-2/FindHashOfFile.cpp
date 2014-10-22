#include "stdafx.h"
#include "FindHashOfFile.h"
#include <Windows.h>
#include <stdio.h>
#define BUFSIZE 1024
#define MD5LEN  16
#include <iostream>
using namespace std;

FindHashOfFile::FindHashOfFile(wchar_t* filename)
{
	this->filename = filename;
}

FindHashOfFile::~FindHashOfFile()
{
	if (logmessage != NULL)
		free(logmessage);
	if (filename != NULL)
		free(filename);
}

void FindHashOfFile::Run()
{
	DWORD dwStatus=0;
	BOOL bResult = FALSE;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	HANDLE hFile = nullptr;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;
	wchar_t rgbDigits[] = L"0123456789abcdef";
	// Logic to check usage goes here.

	hFile = CreateFile(filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		dwStatus = GetLastError();
 
		wprintf(L"Error opening file %s\nError: %d\n", filename,
			dwStatus);
		return;
	}

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv,
		nullptr,
		nullptr,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT))
	{
		dwStatus = GetLastError();
		printf("CryptAcquireContext failed: %d\n", dwStatus);
		CloseHandle(hFile);
		return;
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		dwStatus = GetLastError();
		printf("CryptAcquireContext failed: %d\n", dwStatus);
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return;
	}

	while (bResult = ReadFile(hFile, rgbFile, BUFSIZE,
		&cbRead, nullptr))
	{
		if (0 == cbRead)
		{
			break;
		}

		if (!CryptHashData(hHash, rgbFile, cbRead, 0))
		{
			dwStatus = GetLastError();
			printf("CryptHashData failed: %d\n", dwStatus);
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			return;
		}
	}

	if (!bResult)
	{
		dwStatus = GetLastError();
		printf("ReadFile failed: %d\n", dwStatus);
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return;
	}

	cbHash = MD5LEN;
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		logmessage = static_cast<wchar_t*>(calloc((2*cbHash+2), sizeof(wchar_t)));
		for (DWORD i = 0; i < cbHash; i++)
		{
			logmessage[2*i] = rgbDigits[rgbHash[i] >> 4];
			logmessage[2*i+1] = rgbDigits[rgbHash[i] & 0xf];

		}
		wcscat(logmessage, L"\0");
	}
	else
	{
		dwStatus = GetLastError();
		printf("CryptGetHashParam failed: %d\n", dwStatus);
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(hFile);

	return;
}

void FindHashOfFile::Log()
{
	if (logmessage != NULL)
	{
		printf("%d Hash of %ls is %ls\n", GetCurrentThreadId(), filename, logmessage);
	}
}