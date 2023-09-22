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
    
   
    for (auto dll : dllList) {
        if (wcscmp(dll, dllName) == 0) {
            return true;
        }
    }

    return false;

}


bool EjectDll(const wchar_t* dllName) {
    
    HMODULE tModule = GetModuleHandle(dllName);
    if (tModule == NULL) {
        return false;
    }

    FreeLibrary(tModule);

    return true;
}

int CheckingDll(std::set<const wchar_t*> dllList, PLIST_ENTRY head , std::vector<const wchar_t*> &blackList) {

    bool first = false;
    for (PLIST_ENTRY entry = head->Flink; entry != head; entry = entry->Flink) {
        if (!first) {
            first = true;
            continue;
        }

        PLDR_DATA_TABLE_ENTRY pLdrEntry = CONTAINING_RECORD(entry, LDR_DATA_TABLE_ENTRY, InMemoryOrderModuleList);
        
        
        if (!DllListCheck(dllList, pLdrEntry->BaseDllName.Buffer)) {
            blackList.push_back(pLdrEntry->BaseDllName.Buffer);
            return false;
        }
        
       // wprintf(L" dllName : %s \r\n", pLdrEntry->BaseDllName.Buffer);
    }

    return true;
}



int main() {
    
    PPEB peb = nullptr;
    GetPEB(&peb);
    std::vector<const wchar_t*> blackList;
    std::set<const wchar_t*> dllWhiteList;
    dllWhiteList.insert(L"ntdll.dll");
    dllWhiteList.insert(L"KERNEL32.DLL");
    dllWhiteList.insert(L"KERNELBASE.dll");
    dllWhiteList.insert(L"ucrtbased.dll");
    dllWhiteList.insert(L"MSVCP140D.dll");
    dllWhiteList.insert(L"VCRUNTIME140D.dll");
    CheckingDll(dllWhiteList, &peb->Ldr->InMemoryOrderModuleList,blackList);
    
    for (const wchar_t* dll : blackList) {
        EjectDll(dll);
    }
    
    return 0;
}
