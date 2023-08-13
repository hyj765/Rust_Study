#include"stdafx.h"
#include"regstrymgr.h"

class VMDetector {
private:
	RegMgr* registryManager = nullptr;
public:
	VMDetector();
	bool detectVirtualMachine();
	bool detectVirtualWare();
	bool MacAddressCheck(std::vector<std::vector<int>> prefix);



};