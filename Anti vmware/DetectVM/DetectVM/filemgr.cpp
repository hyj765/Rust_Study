#include "filemgr.h"



std::vector<std::wstring>FileMgr::GetFileList(const wchar_t* &folderPath) {
	std::vector<std::wstring> fileList;
	std::wstring rFolderPath(folderPath);
	rFolderPath += L"\\*";


	WIN32_FIND_DATA FilesData;
	HANDLE findHandle = FindFirstFile(rFolderPath.c_str(),&FilesData);

	if (findHandle != INVALID_HANDLE_VALUE) {
		do {
			if (!(FilesData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				fileList.push_back(FilesData.cFileName);
			}

		} while (FindNextFile(findHandle, &FilesData) != 0);
	}

	return fileList;
}


bool FileMgr::IsExistInFolder(const wchar_t*& folderPath, const wchar_t*& targetFileName) {

	std::vector<std::wstring> fileList = GetFileList(folderPath);
	
	int len = sizeof targetFileName;

	for (std::wstring file : fileList) {
		if (len > sizeof file.c_str()) {
			if (wmemcmp(file.c_str(), targetFileName, sizeof file) == 0) {
				return true;
			}
		}
		else {
			if (wmemcmp(file.c_str(), targetFileName, sizeof targetFileName) == 0) {
				return true;
			}
		}
	}
	return false;
}
