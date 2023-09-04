#pragma once
#include"stdsfx.h"


typedef HMODULE(WINAPI* pLibraryA)(LPCSTR libname);

class Hooker {
private:
	std::set<std::string> whitelist;
	
	struct ApiTable {
		pLibraryA pLib = LoadLibraryA;
	};

public:

	void WinApiInitialize();
	void HookingWinApi(DWORD_PTR HookFunction);
	
};