#pragma once
#include"antiDebugger.h"

AntiDebugger& AntiDebugger::GetInstance() {
	static AntiDebugger AtDebugger;

	return AtDebugger;
}

void AntiDebugger::Initialize() {

	if (this->dbgProcessNames.size() != 0) {
		return;
	}

	const wchar_t* dbgProcList[] = {
		L"gdb.exe",
		L"OllyDbg.exe",
		L"OllyDBG.exe",
		L"OLLYDBG.exe",
		L"ida64.exe",
		L"idaq64.exe",
		L"x64dbg.exe",
		L"windbg.exe",
		L"ImmunityDebugger.exe",
		L"r2.exe",
		L"Hopper.exe",
		L"Cheat Engine.exe",
		L"frida.exe"
	};

	{
		std::lock_guard<std::mutex> lock(this->transactionMutax);
		for (const wchar_t* dbgProcName : dbgProcList) {
			dbgProcessNames.push_back(dbgProcName);
		}
	}

	if (pNtQueryInformation != nullptr) {
		return;
	}

	HMODULE hNtDll = LoadLibraryA("ntdll.dll");
	if (hNtDll) {
		pNtQueryInformation = (pNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
	}

}

bool AntiDebugger::AddDebugProcessList(std::vector<const wchar_t*> procList) {

	try {
		std::lock_guard<std::mutex> lock(this->transactionMutax);
		for (const wchar_t* proc : procList) {
			this->dbgProcessNames.push_back(proc);
		}

	}
	catch(const std::bad_alloc& e){
		
		return false;
	}
	return true;
}

bool AntiDebugger::CheckByDebuggingFlag() {
	return IsDebuggerPresent();
}

BYTE AntiDebugger::PatchDbgBreakPoint() {
	
	// Caution: this function modify "Ntdll" if you use this function you must call UnpatchDbgBreakPoint()
	// UnPatchDbgBreakPoint function required that original first byte of DBGbreakPoint Function. first byte is 0xCC; 

	HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
	if (!hNtdll)
		return -1;

	FARPROC pDbgBreakPoint = GetProcAddress(hNtdll, "DbgBreakPoint");
	if (!pDbgBreakPoint) {
		return -1;
	}

	DWORD dwOldProtect;

	if (!VirtualProtect(pDbgBreakPoint, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect)) {
		return -1;
	}

	BYTE originalByte;

	if (!ReadProcessMemory(GetCurrentProcess(), pDbgBreakPoint, &originalByte, 1, nullptr)) {
		return -1;
	}

	*(PBYTE)pDbgBreakPoint = (BYTE)0xC3;
	
	return originalByte;
}

void AntiDebugger::UnPachDbgBreakPoint(BYTE original) {

	HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
	if (!hNtdll) {
		return;
	}

	FARPROC pDbgBreakPoint = GetProcAddress(hNtdll, "DbgBreakPoint");
	if (!pDbgBreakPoint) {
		return;
	}

	DWORD dwOldProtect;
	if (!VirtualProtect(pDbgBreakPoint, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect)) {
		return;
	}

	*(PBYTE)pDbgBreakPoint = original;
}

bool AntiDebugger::IsFunctionPatchDetect(const char* moduleName, const char* functionName) {

	HMODULE targetModule = GetModuleHandleA(moduleName);
	if (!targetModule) {
		return false;
	}

	FARPROC pTargetFunction = GetProcAddress(targetModule, functionName);
	if (!pTargetFunction) {
		return false;
	}

	HANDLE processSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (processSnapShot) {
		PROCESSENTRY32W procEntry;
		procEntry.dwSize = sizeof(PROCESSENTRY32W);
		const DWORD curPid = GetCurrentProcessId();
		DWORD functionbyte = 0;
		HANDLE hProcess = NULL;
		if (Process32First(processSnapShot, &procEntry)) {
			do {
				if (curPid == procEntry.th32ProcessID) {
					continue;
				}

				hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,procEntry.th32ProcessID);

				if (ReadProcessMemory(hProcess, pTargetFunction, &functionbyte, sizeof(DWORD), NULL)) {
					if (functionbyte != *(PDWORD)pTargetFunction) {
						CloseHandle(hProcess);
						CloseHandle(processSnapShot);
						return true;
					}
				}

			} while (Process32NextW(processSnapShot, &procEntry));
		}
		if (hProcess) {
			CloseHandle(hProcess);
		}
		if (processSnapShot) {
			CloseHandle(processSnapShot);
		}
	}

	return false;
}

bool AntiDebugger::IsDebuggerProcessRunning(const wchar_t* procName) {

	if (this->dbgProcessNames.size() == 0) {
		return false;
	}

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(snapShot, &entry)) {
		do {

			if (wcscmp(entry.szExeFile, procName) == 0) {
				CloseHandle(snapShot);
				return true;
			}
		} while (Process32Next(snapShot, &entry));
	}

	CloseHandle(snapShot);
	return false;
}

int AntiDebugger::IsExistDebuggingProcess(HANDLE processHandle) {
	BOOL pbDebuggingPresnet;

	if (CheckRemoteDebuggerPresent(processHandle, &pbDebuggingPresnet)) {
		if (pbDebuggingPresnet) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return -1;
	}
	
}

bool AntiDebugger::IsCheckProcessInformation() {

	if (pNtQueryInformation == NULL) return false;



}

bool AntiDebugger::CheckDebuggingbyHandle(const wchar_t* fileName) {
	bool flag = (INVALID_HANDLE_VALUE == CreateFileW(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0));
	return flag;
}

bool AntiDebugger::ExceptionBasedCheck() {

	__try {
		__asm {
			int 3
		}
		return true;
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		return false;
	}
}

void AntiDebugger::RegistUnHandlerException(UnhandlerExceptionFilter* unHandlerFunc) {
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)unHandlerFunc);
}

bool AntiDebugger::TimeIntervalCheck(DWORD64 nativeTime, ULARGE_INTEGER start, ULARGE_INTEGER end) {
	
	if (start.QuadPart == NULL || end.QuadPart == NULL) {
		return false;
	}

	return (end.QuadPart - start.QuadPart) > nativeTime;
}

ULARGE_INTEGER AntiDebugger::RecordTime() {

	ULARGE_INTEGER time;

	__asm
	{
		xor ecx, ecx
		rdtsc
		mov time.LowPart, eax
		mov time.HighPart, edx
	}

	return time;
}