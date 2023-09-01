#include"stdafx.h"
#include"antiDebugger.h"


#pragma comment(linker, "/INCLUDE:__tls_used")


void NTAPI tls_callback(PVOID tHandle, DWORD reason, PVOID reserved) {

	printf("tls_callback is running");
	if (AntiDebugger::GetInstance().CheckByDebuggingFlag()) {
		ExitProcess(-1);
	}

}

#pragma data_seg(".CRT$XLX")
PIMAGE_TLS_CALLBACK TLS_CALLBACK[] = { tls_callback,0 };
#pragma data_seg()



int main() {

	
	
}