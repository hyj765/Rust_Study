#include"stdafx.h"



void GetPEB(PPEB* peb) {

#ifndef _WIN64
   *peb = (PPEB)__readfsdword(0x30);
#else
    *peb = (PPEB)__readgsqword(0x60);
#endif 
}

bool ExtensionCheck(const wchar_t* extension) {
    wprintf(L"\n %s \r\n", extension);
    if (wcslen(extension) != 3) return false;

    if (wcscmp(extension, L"DLL") != 0 || wcscmp(extension, L"dll") != 0) return false;
    
        return true;
}

bool DllListCheck(std::set<const wchar_t*> dllList,const wchar_t* dllName) {
    
    std::set<const wchar_t*>::iterator itor = dllList.find(dllName);

    if (itor != dllList.end()) return true;
    
    return false;

}


bool EjectDll(char* dllList) {
    
    return true;
}

int CheckingDll(std::set<const wchar_t*> dllList, PLIST_ENTRY head ) {

    bool first = false;
    for (PLIST_ENTRY entry = head->Flink; entry != head; entry = entry->Flink) {
        if (!first) {
            first = true;
            continue;
        }
        PLDR_DATA_TABLE_ENTRY pLdrEntry = CONTAINING_RECORD(entry, LDR_DATA_TABLE_ENTRY, InMemoryOrderModuleList);
        
        if (!DllListCheck(dllList, pLdrEntry->BaseDllName.Buffer)) {
            return false;
        }

        std::cout << pLdrEntry->BaseDllName.MaximumLength << " " << pLdrEntry->BaseDllName.Length << " ";
        wprintf(L" dllName : %s \r\n", pLdrEntry->BaseDllName.Buffer);
        //int len = sizeof(dllList);
        //for(int len = )
    }

    return 1;
}



int main() {
    // PEB 구조체의 포인터 선언
    
    PPEB peb = nullptr;
    GetPEB(&peb);
    std::set<const wchar_t*> dllList;
    CheckingDll(dllList, &peb->Ldr->InMemoryOrderModuleList);
    

    
    return 0;
}
