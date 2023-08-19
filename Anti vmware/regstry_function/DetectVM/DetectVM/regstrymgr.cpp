#include"regstrymgr.h"



wchar_t * RegMgr::convertASCII(const char* charString) {

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wideString = converter.from_bytes(charString);
	wchar_t* wcstring = new wchar_t[wideString.length() + 1];
	wmemcpy_s(wcstring, wideString.length() + 1, wideString.c_str(), wideString.length() + 1);
	wcstring[wideString.length()] = L'\0';
	return wcstring;
}


HKEY RegMgr::RegstryOpen(HKEY key, const char* subkey) {
	HKEY regList=NULL;
	wchar_t* convertedSubKey = this->convertASCII(subkey);
	LSTATUS status = RegOpenKeyEx(key, convertedSubKey, 0, KEY_READ, &regList);
	if (status == ERROR_SUCCESS){
		delete[] convertedSubKey;
		return regList;;
	}

	delete[] convertedSubKey;
	return NULL;
}


/*
LONG RegEnumValue(
  HKEY    hKey, 얻어온 Hkey value
  DWORD   dwIndex, start index
  LPWSTR  lpValueName, 저장될 버퍼
  LPDWORD lpcchValueName, 버퍼 크기
  LPDWORD lpReserved, 사용하지 않음
  LPDWORD lpType, 사용하지 않음
  LPBYTE  lpData, 열거 값이 저장될 버퍼
  LPDWORD lpcbData 
);



*/


void RegMgr::RegstrySelectALL(HKEY key, const char* subkey) {
	HKEY regList = NULL;
	regList = this->RegstryOpen(key, subkey);
	if (regList == NULL) {
		wprintf(L"Wrong key Input or fail to access the registry key\n");
		return;
	}
	wchar_t buffer[MAX_PATH];
	DWORD buffsiz = MAX_PATH;
	BYTE data[MAX_PATH];
	DWORD datasiz = MAX_PATH;
	DWORD index = 0;
	DWORD type;
	while (RegEnumValue(regList, index, buffer, &buffsiz, NULL, &type, data,&datasiz) == ERROR_SUCCESS) {
		if (type == REG_SZ) {
			wprintf(L"keyName : %s, value data: %s\n", buffer, reinterpret_cast<const wchar_t*>(data));
		}
		else if (type == REG_DWORD) {
			wprintf(L"keyName : %s, value data: %d\n", buffer, *reinterpret_cast<const DWORD*>(data));
		}
		index++;
		buffsiz = MAX_PATH;
		datasiz = MAX_PATH;
	}

	RegCloseKey(regList);
	return;
}

BYTE* RegMgr::RegstrySelect(HKEY key, const char* subkey, const char* selectkey) {
	DWORD dataSize = 0;
	RegGetValue(key, convertASCII(subkey), convertASCII(selectkey), RRF_RT_ANY, NULL, NULL, &dataSize);

	BYTE* buffer = new BYTE[dataSize];

	if (buffer) {
		if (RegGetValue(key, convertASCII(subkey), convertASCII(selectkey), RRF_RT_ANY, NULL, buffer, &dataSize) == ERROR_SUCCESS) {
			return buffer;
		}
		else {
			delete[] buffer;
		}
	}

	return nullptr;
}
