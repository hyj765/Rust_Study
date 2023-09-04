#include"FunctionHooker.h"



void Hooker::HookingWinApi(DWORD_PTR HookFunction) {
	

}

void Hooker::WinApiInitialize() {

	LPVOID imagebase = GetModuleHandleA(NULL);
	PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(imagebase);
	PIMAGE_NT_HEADERS ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>((DWORD_PTR)imagebase + dosHeader->e_lfanew);
	PIMAGE_IMPORT_DESCRIPTOR iid = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD_PTR)imagebase + ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	
	while (iid->Name != NULL) {
		
		LPCSTR libraryName = (LPCSTR)iid->Name + (DWORD_PTR)imagebase;
		HMODULE library = LoadLibraryA(libraryName);
		
		if (library) {
			PIMAGE_THUNK_DATA originalFirstThunk=NULL, firstThunk=NULL;
			originalFirstThunk = (PIMAGE_THUNK_DATA)((DWORD_PTR)imagebase + iid->OriginalFirstThunk);
			firstThunk = (PIMAGE_THUNK_DATA)((DWORD_PTR)imagebase + iid->FirstThunk);
			
			while (originalFirstThunk->u1.AddressOfData != NULL) {

				PIMAGE_IMPORT_BY_NAME functionName = (PIMAGE_IMPORT_BY_NAME)((DWORD_PTR)imagebase + originalFirstThunk->u1.AddressOfData);
				
				
				++originalFirstThunk;
				++firstThunk;
			}
			
		}


		iid++;
	}

}

