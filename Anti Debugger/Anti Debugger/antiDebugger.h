#pragma once
#include"stdafx.h"

typedef NTSTATUS(WINAPI* pNtQueryInformationProcess)(
	HANDLE processHandle,
	PPROCESS_INFORMATION processInforamtionClass,
	PVOID ProcessInformation,
	PULONG ReturnLength
	);

class AntiDebugger {
	std::vector<const wchar_t*> dbgProcessNames;
	pNtQueryInformationProcess pNtQueryInformation;
public:
	void Initialize();
	bool CheckByDebuggingFlag();
	bool IsDebuggerProcessRunning(const wchar_t* procName);
	int IsExistDebuggingProcess(HANDLE processHandle);
	bool IsCheckProcessInformation();
};