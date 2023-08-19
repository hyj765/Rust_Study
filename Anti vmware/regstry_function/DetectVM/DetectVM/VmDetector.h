#pragma once
#include"stdafx.h"
#include"regstrymgr.h"
#include"filemgr.h"
#include<tlhelp32.h>

class VMDetector {
private:
	RegMgr registryManager;
	FileMgr fileManager;
public:
	VMDetector() {};
	bool RegistryCheck(HKEY mainkey,const char* registry);
	bool RegistryValueCheck(HKEY mainkey, const char* subkey, const char* registryValueKey, const char* cmpValue);
	bool DetectVirtualBox();
	bool DetectVirtualWare();
	bool MacAddressCheck(const std::vector<std::vector<int>> &prefix);
	bool HardwareIDBasedCheck(const std::vector<DWORD> &CPUIDS,const std::vector<DWORD> &checkList);
	std::vector<DWORD> RetCPUID();
	bool FileBasedCheck(const wchar_t* folderPath, const wchar_t* targetFileName);
	bool ProcessBasedCheck(const wchar_t* targetProcessName);
	bool IOPortBasedDetection();

};