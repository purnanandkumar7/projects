#include "stdafx.h"
#include <SDKDDKVer.h>
#include <iostream>
#include <windows.h>
#include <WinIoCtl.h>
#include <stdio.h>
#include <string>

using namespace std;

int main(int argc, TCHAR *argv[])
{
	DWORD dwRead;
	char DataBuffer[512];
	DWORD dwBytesToWrite = 512;
	DWORD dwBytesWritten = 0;
	unsigned int _nsect = 0;
	wstring wsPath;
	wstring wsPrefix = L"";

	wcout << "\n\nEnter Path: ";
	wcin >> wsPath;
	wstring lpPath = wsPrefix + wsPath;

	HANDLE hDisk = CreateFile(L"\\\\.\\PhysicalDrive0",
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hDisk == INVALID_HANDLE_VALUE)
	{
		wcout << "CreateFile error " << GetLastError() << endl;
		return 0;
	}

	SetFilePointer(hDisk, _nsect * 512, 0, FILE_BEGIN);

	BOOL bErrorRead = ReadFile(hDisk,
		DataBuffer,
		dwBytesToWrite,
		&dwRead,
		NULL);

	if (FALSE == bErrorRead)
	{
		wcout << "Terminal failure: Unable to read to file" << GetLastError() << endl;
		CloseHandle(hDisk);
		return 0;
	}


	HANDLE hFile = CreateFile(lpPath.c_str(),
		GENERIC_READ |
		GENERIC_WRITE,
		FILE_SHARE_READ | // share mode
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		wcout << "CreateFile error " << GetLastError() << endl;
		return 0;
	}

	DeviceIoControl(
		hFile,
		FSCTL_LOCK_VOLUME,
		NULL,
		0,
		NULL,
		0,
		NULL,
		NULL
	);

	BOOL bErrorWrite = WriteFile(
		hFile,
		DataBuffer,
		dwBytesToWrite,
		&dwBytesWritten,
		NULL);

	if (FALSE == bErrorWrite)
	{
		wcout << "Terminal failure: Unable to write to file" << GetLastError() << endl;
	}
	else
	{
		if (dwBytesWritten != dwBytesToWrite)
		{
			wcout << "Error: dwBytesWritten != dwBytesToWrite" << endl;
		}
		else
		{
			wcout << "Wrote " << dwBytesWritten << " bytes successfully" << endl;
		}
	}

	DeviceIoControl(
		hFile,
		FSCTL_UNLOCK_VOLUME,
		NULL,
		0,
		NULL,
		0,
		NULL,
		NULL
	);


	CloseHandle(hFile);

	CloseHandle(hDisk);

	getchar();

}