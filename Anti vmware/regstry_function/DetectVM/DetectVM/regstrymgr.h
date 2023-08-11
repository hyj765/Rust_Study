#pragma once
#include"stdafx.h"



class RegMgr {
public:
	wchar_t* convertASCII(const char* charString);
	HKEY RegstryOpen(HKEY key,const char* subkey);
	void RegstryClose() {};
	void RegstrySelectALL(HKEY key, const char* subkey);
	void RegstrySelect(HKEY key, const char* subkey);
};