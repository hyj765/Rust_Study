#include"regstrymgr.h"



wchar_t * RegMgr::convertASCII(const char* charString) {
	// const char[]�� wchar_t[]�� ��ȯ
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
	if (RegOpenKeyEx(key, convertedSubKey, 0, KEY_ALL_ACCESS, &regList) == ERROR_SUCCESS) {
		delete[] convertedSubKey;
		return regList;
	}

	delete[] convertedSubKey;
	return NULL;
}


/*
LONG RegEnumValue(
  HKEY    hKey, ���� Hkey value
  DWORD   dwIndex, start index
  LPWSTR  lpValueName, ����� ����
  LPDWORD lpcchValueName, ���� ũ��
  LPDWORD lpReserved, ������� ����
  LPDWORD lpType, ������� ����
  LPBYTE  lpData, ���� ���� ����� ����
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
	wchar_t buffer[256];
	DWORD buffsiz = 256;
	BYTE data[16384];
	DWORD datasiz = 16384;
	DWORD index = 0;
	DWORD type;
	while (RegEnumValue(regList, index, buffer, &buffsiz, NULL, &type, data,&datasiz) == ERROR_SUCCESS) {
		wprintf(L"keyName : %s, value data: %s\n", buffer, data);
		
		index++;
		buffsiz = 256;
		datasiz = 16384;
	}

	RegCloseKey(regList);
	return;
}


bool RegMgr::RegstrySelect(HKEY key, const char* subkey, const char* selectkey) {
	
	HKEY regList = NULL;
	regList = this->RegstryOpen(key, subkey);
	if (regList == NULL) {
		return false;
	}
	
	char buffer[265];
	DWORD getByte;
	RegQueryValueExA(regList, selectkey,NULL,NULL,(LPBYTE)buffer,&getByte);

	printf("%s", buffer);
	RegCloseKey(regList);
	return true;
}