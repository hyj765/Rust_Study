#include<Windows.h>
#include <iostream>
#include<stdlib.h>
#include<DbgHelp.h>

#pragma comment(lib,"Dbghelp.lib")


//memoryDump with WinAPI, MiniDumpWriteDump
void CreateMiniDump(const DWORD pid) {

    HANDLE hProcess,hFile;
    bool success;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS,0,pid);
    if (hProcess == INVALID_HANDLE_VALUE) {
        printf("fail API OpenProcess\n");
        return;
    }

    hFile = CreateFileA(".\\memdump.dmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL );
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("CreateFile fail %d\n",GetLastError());
        CloseHandle(hProcess);
        return;
    }

    success = MiniDumpWriteDump(hProcess, pid, hFile, MiniDumpWithFullMemory, NULL, NULL, NULL);
    if (!success) {
        printf("fail to minidump\n");
    }

    CloseHandle(hFile);
    CloseHandle(hProcess);
    
}



int main(int argc, const char* argv[]) {

    if (argc != 2) {
        printf("[USAGE] TargetProcessId ");
        return 0;
    }
    DWORD pid = atoi(argv[1]);
    CreateMiniDump(pid);
 
    
    return 0;
}
