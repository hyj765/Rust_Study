#pragma once
#include"stdafx.h"



class RegMgr {
public:
	wchar_t* convertASCII(const char* charString);
	HKEY RegstryOpen(HKEY key,const char* subkey);
	void RegstrySelectALL(HKEY key, const char* subkey);
	bool RegstrySelect(HKEY key, const char* subkey, const char* selectkey);
};