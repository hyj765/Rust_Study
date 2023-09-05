#include<iostream>
#include<Windows.h>
#include"FunctionHooker.h"

using RestrictedLoadLibary = HMODULE(WINAPI*)(LPCSTR lplibfileName);
RestrictedLoadLibary originalLib = LoadLibraryA;

HMODULE hookedLoadLib(LPCSTR lplibfileName) {

	if (lplibfileName == "Inject.dll") {
		std::cout << "is blocked" << std::endl;
		return NULL;
	}

	return originalLib(lplibfileName);
}

int main()
{
	Hooker s;
	s.WinApiInitialize();
	s.HookingWinApi("LoadLibraryA", (DWORD_PTR)hookedLoadLib);

	LoadLibraryA("Inject.dll");
	
	return 0;
}
