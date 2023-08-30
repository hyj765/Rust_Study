#pragma once
#include"stdafx.h"
#include"IntegrityChecker.h"
typedef NTSTATUS(WINAPI* pNtQueryInformationProcess)(
	HANDLE processHandle,
	PPROCESS_INFORMATION processInforamtionClass,
	PVOID ProcessInformation,
	PULONG ReturnLength
	);

typedef LONG (UnhandlerExceptionFilter)(PEXCEPTION_POINTERS pExceptionInfo);


class AntiDebugger {
private:
	AntiDebugger() {};
	AntiDebugger(const AntiDebugger& ref) {};
	AntiDebugger& operator=(const AntiDebugger& ref);
	~AntiDebugger() {};
	std::vector<const wchar_t*> dbgProcessNames;
	pNtQueryInformationProcess pNtQueryInformation = nullptr;
	std::mutex transactionMutax;
	IntegrityChecker integrityCheker;
public:
	static AntiDebugger& GetInstance();
	void Initialize();
	bool AddDebugProcessList(std::vector<const wchar_t*> procList);
	bool CheckByDebuggingFlag();
	bool IsFunctionPatchDetect(const char* moduleName,const char* functionName);
	BYTE PatchDbgBreakPoint();
	void UnPachDbgBreakPoint(BYTE original);
	bool IsDebuggerProcessRunning(const wchar_t* procName);
	int IsExistDebuggingProcess(HANDLE processHandle);
	bool IsCheckProcessInformation();
	bool CheckDebuggingbyHandle(const wchar_t* fileName);
	bool ExceptionBasedCheck();
	void RegistUnHandlerException(UnhandlerExceptionFilter* unHandlerFunc);
	bool TimeIntervalCheck(DWORD64 nativeTime, ULARGE_INTEGER start, ULARGE_INTEGER end);
	ULARGE_INTEGER RecordTime();
	
};