#pragma once
#include"antiDebugger.h"


void AntiDebugger::Initialize() {

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

	for (const wchar_t* dbgProcName : dbgProcList) {
		dbgProcessNames.push_back(dbgProcName);
	}

	HMODULE hNtDll = LoadLibraryA("ntdll.dll");
	if (hNtDll) {
		pNtQueryInformation = (pNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
	}

}

bool AntiDebugger::CheckByDebuggingFlag() {
	return IsDebuggerPresent();
}

bool AntiDebugger::IsDebuggerProcessRunning(const wchar_t* procName) {

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

