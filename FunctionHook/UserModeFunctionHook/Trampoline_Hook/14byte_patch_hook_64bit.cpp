#include<iostream>
#include<Windows.h>
using namespace std;

#pragma pack(push,1)
struct hookObject { // hook struct 
	BYTE opcode1 = 0x68; // push
	DWORD lptarget; // proc low address
	DWORD opcode2 = 0x042444C7; // rsp+4 ,
	DWORD lptarget2; // proc high address
	BYTE opcode3 = 0xC3; // ret
};
#pragma pack(pop)



int WINAPI MyMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) {
	return MessageBoxW(hWnd, L"hooked", L"hooked",MB_OK);
}

BYTE originalBuf[14] = { 0 };

DWORD WINAPI hook64() {
	
	HMODULE hModule = LoadLibraryA("user32.dll");
	LPVOID Original=(LPVOID)GetProcAddress(hModule, "MessageBoxA");
	DWORD oldProtect;
	hookObject hookobj;
	VirtualProtect(Original, sizeof(hookObject), PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy_s(&originalBuf, sizeof(hookObject), Original, sizeof(hookObject));
	
	hookobj.lptarget = (DWORD)((DWORD64)&MyMessageBoxA & 0xFFFFFFFF); 
	hookobj.lptarget2 = (DWORD)((DWORD64)&MyMessageBoxA >> 32); //  only half data remaining
	
	memcpy_s(Original, sizeof(hookObject), &hookobj, sizeof(hookObject));
	VirtualProtect(Original, sizeof(hookObject), oldProtect, NULL);

	return true;
}


int main() {

	hook64();
	
	MessageBoxA(NULL,"commonWork","commonWork",MB_OK);

	

}