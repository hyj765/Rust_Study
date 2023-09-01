#pragma once
#include"stdafx.h"

class IntegrityChecker {

public:
	size_t GetFunctionSize(PVOID functionaddress);
	const unsigned char* GetDataIntoHash(const unsigned char* hash,std::string data);
	std::string GetFuntionData(PVOID functionAddress, size_t functionSiz);
	std::string EncryptionSha256(std::string data);

};