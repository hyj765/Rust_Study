#include"IntegrityChecker.h"




size_t IntegrityChecker::GetFunctionSize(PVOID functionaddress) {
	
	PBYTE pMemory = (PBYTE)functionaddress;
	size_t functionSiz = 0;

	do {


		++functionSiz;
	} while (*(pMemory++) != 0x3C);

	return functionSiz;
}

const unsigned char* IntegrityChecker::GetDataIntoHash(const unsigned char* hash, std::string data) {

	return NULL;
}

std::string IntegrityChecker::GetFuntionData(PVOID functionAddress, size_t functionSiz) {
	
	std::string functionData;
	PBYTE pMemory = (PBYTE)functionAddress;
	while (functionSiz != 0) {
		functionData.push_back(*(pMemory++));
		functionSiz--;
	}


	return functionData;
}

std::string IntegrityChecker::EncryptionSha256(std::string data) {
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char*)data.c_str(), data.size(), hash);

	std::stringstream ss;

	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}
	
	return ss.str();
}