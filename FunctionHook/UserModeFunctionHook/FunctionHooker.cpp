#include"FunctionHooker.h"



void Hooker::HookingWinApi(std::string functionName, DWORD_PTR function) {
	
	DWORD old = 0;
	// IAT의 주소 값에 권한 변경
	VirtualProtect((LPVOID)(apioriginal[functionName]), 8, PAGE_READWRITE, &old);
	// IAT의 값은 해당 dll 함수의 위치주소를 담고 있기 때문에 이를 내가 만든 함수로 변경
	*(apioriginal[functionName]) = (DWORD_PTR)function;

}

void Hooker::WinApiInitialize() {

	// HModule은 handle이 아닌 imagebase 값을 반환함.
	LPVOID imagebase = GetModuleHandleA(NULL);
	
	// dos header 확보
	PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(imagebase);
	//nt header 확보
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
				// apioriginal이라는 map에 해당 iat의 주소 값 저장
				// 이렇게 수행하면 매번 모든 iat를 순회하지 않고 한번만에 api들을 사용할 수 있음.
				apioriginal[std::string(functionName->Name)] = &firstThunk->u1.Function;
				++originalFirstThunk;
				++firstThunk;
			}
			
		}
		iid++;
	}
}

