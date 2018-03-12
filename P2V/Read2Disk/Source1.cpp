#include<iostream>
#include<Windows.h>
using namespace std;
HANDLE clcLicenseImport();
int main()

{
	cout << clcLicenseImport() << endl;
}
HANDLE clcLicenseImport()
{
	HANDLE license;
	wstring Path;
	wstring wsPrefix = L"";
	wcout << "\n\nEnter Path: ";
	wcin >> Path;
	Path = wsPrefix + Path;

	license = CreateFile(Path.c_str(),               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (license == INVALID_HANDLE_VALUE)
	{
		printf("Could not open file (error %d\n)", GetLastError());
		return 0;
	}
	return license;
}