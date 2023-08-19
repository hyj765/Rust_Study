#pragma once
#include "stdafx.h"


class FileMgr {
public:
	std::vector<std::wstring>GetFileList(const wchar_t* &folderPath);
	bool IsExistInFolder(const wchar_t*& folderPath, const wchar_t*& targetFileName);
};