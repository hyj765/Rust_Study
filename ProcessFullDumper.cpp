#include<iostream>
#include<Windows.h>
#include<minidumpapiset.h>
#include<string>

#pragma comment(lib, "Dbghelp.lib")


void CreateFullDump(DWORD processId, const std::string& dumpPath)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (hProcess == NULL)
	{
		printf("fail to get process handle\n");

		return;
	}

	HANDLE hFile = CreateFileA(dumpPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("fail to CreateFileA %d\n", GetLastError());
		return;
	}

	MINIDUMP_EXCEPTION_INFORMATION minidumpInfo;
	minidumpInfo.ThreadId = GetCurrentThreadId();
	minidumpInfo.ExceptionPointers = nullptr;
	minidumpInfo.ClientPointers = false;

	if (MiniDumpWriteDump(hProcess, processId, hFile, MiniDumpWithFullMemory, &minidumpInfo, NULL, NULL) == true)
	{
		printf("dump sucessfully done\n");
	}
	else
	{
		printf("%d error", GetLastError());

	}


	CloseHandle(hFile);
	CloseHandle(hProcess);
}


int main(int argc, char* argv[])
{

	if (argc < 3)
	{
		printf("process memory dumper USAGE[processId, dumpFilePath]");
		return 0;
	}

	CreateFullDump(std::stoi(argv[1]), argv[2]);

}
