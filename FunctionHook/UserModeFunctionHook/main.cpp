#include<iostream>
#include<Windows.h>
#include"FunctionHooker.h"

using PrototypeMessageBox = int (WINAPI*)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
PrototypeMessageBox original = MessageBoxA;

int main()
{
    Hooker keeper;

    keeper.WinApiInitialize();
   
    std::cout << original << std::endl;
    keeper.value_print("MessageBoxA","USER32.dll");
    return 0;
}
