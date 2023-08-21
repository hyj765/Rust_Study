#include "VmDetector.h"

bool VMDetector::DetectVirtualBox() {

	const char* checkList[] = {
		"hardware\\acpi\\dsdt\\vbox",
		"hardware\\acpi\\fadt\\vbox",
		"hardware\\acpi\\rsdt\\vbox",
		"hardware\\acpi\\ssdt\\vbox",
		"system\\controlset001\\services\\vboxguest",
		"system\\controlset001\\services\\vboxmouse",
		"system\\controlset001\\services\\vboxservice",
		"system\\controlset001\\services\\vboxsf",
		"system\\controlset001\\services\\vboxwddm"
	};

	int len = sizeof checkList / sizeof checkList[0];
	for (int i = 0; i < len; ++i) {
		if (!RegistryCheck(HKEY_LOCAL_MACHINE, checkList[i])) {
			return false;
		}
	}


	
	const char* cmpValues[] = {
		"VBOX",
		"Oracle VM VirtualBox Version"
	};
	const char* ValueKey[] = {
		"SystemBiosVersion",
		"VideoBiosVersioin"
	};

	for (int i = 0; i < 2; ++i) {
		RegistryValueCheck(HKEY_LOCAL_MACHINE,"software\\description\\system", ValueKey[i],cmpValues[i]);
	}

	const wchar_t* processList[] = {
		L"VBoxService.exe",
		L"VBoxTray.exe"
	};

	for (int i = 0; i < 2; ++i) {
		if (!ProcessBasedCheck(processList[i])) {
			return false;
		}
	}

	const wchar_t* folderPath = L"C:\\WINDOWS\\system32";
	const wchar_t* fileNames[] = {
		L"vboxdisp.dll",
		L"vboxhook.dll",
		L"vboxmrxnp.dll",
		L"vboxogl.dll",
		L"vboxoglarrayspu.dll",
		L"vboxoglcrutil.dll",
		L"vboxoglfeedbackspu.dll",
		L"vboxoglpackspu.dll",
		L"vboxoglpassthroughspu.dll",
		L"vboxservice.exe",
		L"vboxtray.exe",
		L"VBoxControl.exe"
	};

	for (const wchar_t* fileName : fileNames) {
		if (!FileBasedCheck(folderPath, fileName)) {
			return false;
		}
	}

	std::vector<std::vector<int>> prefixaddr = {
	   {0x08, 0x00, 0x27}
	};
	
	if (!MacAddressCheck(prefixaddr)) {
		return false;
	}
	
	return true;
}

bool VMDetector::RegistryCheck(HKEY mainkey,const char* registry) {

	bool flag = true;

	if (registryManager.RegstryOpen(mainkey, registry) != NULL) {
		flag = false;
	}
	return flag;
}

bool VMDetector::RegistryValueCheck(HKEY mainkey,const char* subkey,const char* registryValueKey,const char* cmpValue) {

	bool flag = true;
	BYTE* keyValue;
	keyValue = registryManager.RegstrySelect(mainkey, subkey, registryValueKey);
	if(keyValue != NULL){
		if (memcmp(keyValue, registryManager.convertASCII(cmpValue), sizeof cmpValue) == 0) {
			flag = false;
		}
		delete[] keyValue;
	}
	return flag;
}


bool VMDetector::DetectVirtualWare() {
	
	bool flag = true;

	std::vector<DWORD> CPUIDS= RetCPUID();
	if (CPUIDS.size() == 0) return false;
	else {
		flag = HardwareIDBasedCheck(CPUIDS, { 0x61774d56,0x4d566572,0x65726177 });
	}
	if (!flag) return false; 

	std::vector<std::vector<int>> prefixarr = {
	   {0x00, 0x05, 0x69},
	   {0x00, 0x0C, 0x29},
	   {0x00, 0x1C, 0x14},
	   {0x00, 0x50, 0x56}
	};
	flag=MacAddressCheck(prefixarr);
	if (!flag) {
		return false;
	}

	return true;
}

bool VMDetector::HardwareIDBasedCheck(const std::vector<DWORD>& CPUIDS, const std::vector<DWORD>& checkList) {
	if (CPUIDS[0] == checkList[0] && CPUIDS[1] == checkList[1] && CPUIDS[2] == checkList[2]) {
		return false;
	}
	return true;
}


bool VMDetector::MacAddressCheck(const std::vector<std::vector<int>> &prefixarr) {
	IP_ADAPTER_INFO macInfo[16];
	DWORD dwBufLen = sizeof(macInfo);

	DWORD dwStatus = GetAdaptersInfo(macInfo, &dwBufLen);
	if (dwStatus != ERROR_SUCCESS) {
		return false;
	}

	PIP_ADAPTER_INFO pMacAddressInfo = macInfo;
	while (pMacAddressInfo) {

		for (auto prefix : prefixarr) {
			if (prefix[0] == pMacAddressInfo->Address[0] && prefix[1] == pMacAddressInfo->Address[1] && prefix[2] == pMacAddressInfo->Address[2]) {
				return false;
			}
		}
		
		pMacAddressInfo = pMacAddressInfo->Next;
	}
	
	return true;
}

std::vector<DWORD> VMDetector::RetCPUID() {

	DWORD id1, id2, id3;

	__asm {

		mov eax, 0x1
		cpuid
		mov eax, 0x40000000
		cpuid
		mov id1, ebx
		mov id2, ecx
		mov id3, edx

	}

	return { id1,id2,id3 };
}

bool VMDetector::FileBasedCheck(const wchar_t* folderPath,const wchar_t* targetFileName) {
	
	if (fileManager.IsExistInFolder(folderPath, targetFileName)) {
		return false;
	}

	return true;
}

bool VMDetector::ProcessBasedCheck(const wchar_t* targetProcessName) {

	HANDLE processSnapshot = NULL;
	PROCESSENTRY32 entry = { 0 };
	processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if (processSnapshot == (HANDLE)-1) {
		return false;
	}

	entry.dwSize = sizeof(PROCESSENTRY32);
	
	if (Process32First(processSnapshot, &entry)) {
		do {
			if (wmemcmp(targetProcessName,entry.szExeFile,wcslen(targetProcessName)) == 0) {
				CloseHandle(processSnapshot);
				return false;
			}
		} while (Process32Next(processSnapshot, &entry));
	}
	CloseHandle(processSnapshot);

	return true;
}

bool VMDetector::IOPortBasedDetection() {
	
	int v1, v2;

	__try {
		__asm {

			push eax
			push ebx
			push ecx
			push edx

			mov eax, 'VMXh'
			mov ebx, 0
			mov ecx, 0Ah
			mov edx, 'VX'

			in eax, dx;

			mov v1, ebx
			mov v2, ecx

			pop edx
			pop ecx
			pop ebx
			pop eax

		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {

		return false;
	}

	return true;
}