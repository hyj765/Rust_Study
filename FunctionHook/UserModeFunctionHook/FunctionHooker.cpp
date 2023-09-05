#include"FunctionHooker.h"



void Hooker::HookingWinApi(std::string functionName, DWORD_PTR function) {
	
	SIZE_T bytesWritten = 0;
	DWORD oldProtect = 0;
	// IAT�� �ּ� ���� ���� ����
	VirtualProtect((LPVOID)(apioriginal[functionName]), 8, PAGE_READWRITE, &oldProtect);
	// IAT�� ���� �ش� dll �Լ��� ��ġ�ּҸ� ��� �ֱ� ������ �̸� ���� ���� �Լ��� ����
	*(apioriginal[functionName]) = (DWORD_PTR)function;

}

void Hooker::WinApiInitialize() {

	// HModule�� handle�� �ƴ� imagebase ���� ��ȯ��.
	LPVOID imagebase = GetModuleHandleA(NULL);
	
	// dos header Ȯ��
	PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(imagebase);
	//nt header Ȯ��
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
				// apioriginal�̶�� map�� �ش� iat�� �ּ� �� ����
				// �̷��� �����ϸ� �Ź� ��� iat�� ��ȸ���� �ʰ� �ѹ����� api���� ����� �� ����.
				apioriginal[std::string(functionName->Name)] = &firstThunk->u1.Function;
				++originalFirstThunk;
				++firstThunk;
			}
			
		}
		iid++;
	}
}

