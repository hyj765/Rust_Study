#pragma once
#include"stdsfx.h"


typedef HMODULE(WINAPI* pLibraryA)(LPCSTR libname);

class Hooker {
private:
	std::set<std::string> whitelist;
	std::map<std::string, DWORD*> apioriginal;

public:
	void WinApiInitialize();
	void HookingWinApi(std::string functionName, DWORD_PTR function);
	
};