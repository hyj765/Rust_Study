#include"stdafx.h"



void GetPEB(PPEB* peb) {

#ifndef _WIN64
   *peb = (PPEB)__readfsdword(0x30);
#else
    *peb = (PPEB)__readgsqword(0x60);
#endif 
}


int main() {
    // PEB 구조체의 포인터 선언
    PPEB peb = nullptr;
    GetPEB(&peb);

    PLIST_ENTRY pEntry = &peb->Ldr->InMemoryOrderModuleList;
    while (pEntry->Flink != &peb->Ldr->InMemoryOrderModuleList) {
        
       PLDR_DATA_TABLE_ENTRY pLdrEntry = CONTAINING_RECORD(pEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderModuleList);
       wprintf(L"%s \r\n", pLdrEntry->FullDllName.Buffer);
        pEntry = pEntry->Flink;
    }
    
    return 0;
}
