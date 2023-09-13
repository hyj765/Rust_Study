#include<iostream>
#include<Windows.h>
using namespace std;

bool Trampoline_Hook(BYTE* src, BYTE* dst) {

	DWORD oldProtect;
	if (src[0] == 0xE9) {
		return false;
	}

	VirtualProtect(src, 5, PAGE_EXECUTE_READWRITE,&oldProtect);

	uintptr_t relativeAddress = dst - src - 5;

	*src = 0xE9;
	memcpy(&src[1], &relativeAddress, 4);
	VirtualProtect(src, 5, oldProtect, NULL);
	
	return true;
}


int WINAPI hooked_messageboxA(_In_opt_ HWND hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_ UINT uType) {

	return MessageBoxW(NULL, L"hooked", L"hooked", MB_OK);
}


int main() {
	
	HMODULE hmodule=LoadLibraryA("user32.dll");
	FARPROC src = (FARPROC)GetProcAddress(hmodule, "MessageBoxA");
	Trampoline_Hook((BYTE*)src, (BYTE*)hooked_messageboxA);
	
	MessageBoxA(NULL, "hello", "normal", MB_OK);

}