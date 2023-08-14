#include"stdafx.h"
#include"regstrymgr.h"

class VMDetector {
private:
	RegMgr registryManager;
public:
	VMDetector() {};
	bool DetectVirtualMachine();
	bool DetectVirtualWare();
	bool MacAddressCheck(const std::vector<std::vector<int>> &prefix);
	bool HardwareIDBasedCheck(const std::vector<DWORD> &CPUIDS,const std::vector<DWORD> &checkList);
	std::vector<DWORD> RetCPUID();
	bool IOPortBasedDetection();
};