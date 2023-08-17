#include"stdafx.h"
#include"regstrymgr.h"

class VMDetector {
private:
	RegMgr registryManager;
public:
	VMDetector() {};
	bool RegistryCheck(HKEY mainkey,const char* registry);
	bool RegistryValueCheck(HKEY mainkey, const char* subkey, const char* registryValue);
	bool DetectVirtualBox();
	bool DetectVirtualWare();
	bool MacAddressCheck(const std::vector<std::vector<int>> &prefix);
	bool HardwareIDBasedCheck(const std::vector<DWORD> &CPUIDS,const std::vector<DWORD> &checkList);
	std::vector<DWORD> RetCPUID();
	bool FileBasedCheck();
	bool ProcessBasedCheck();
	bool IOPortBasedDetection();

};