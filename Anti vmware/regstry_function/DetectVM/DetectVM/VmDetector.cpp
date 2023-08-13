#include "VmDetector.h"


VMDetector::VMDetector() {
	this->registryManager = new RegMgr();
}

VMDetector::~VMDetector() {
	delete registryManager;
}

bool VMDetector::detectVirtualMachine() {

	bool flag=registryManager->RegstrySelect(HKEY_LOCAL_MACHINE, "SOFTWARE\\oracle","virtualboxguestaddition");
	if (flag) {
		return false;
	}

	std::vector<std::vector<int>> prefixaddr = {
	   {0x08, 0x00, 0x27}
	};
	
	flag = MacAddressCheck(prefixaddr);
	if (!flag) {
		return false;
	}


	return true;
}

bool VMDetector::detectVirtualWare() {
	
	bool flag = true;

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

bool VMDetector::MacAddressCheck(std::vector<std::vector<int>> prefixarr) {
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
