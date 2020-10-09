#pragma once
#include "env.h"
#include "str.h"

namespace PATH {
	CString Prefix(const CString& strFileName);
	CString Suffix(const CString& strFileName);
	CString Basename(const CString& strFullPath);
	CString Dirname(const CString& strFullPath);
	int Rename(const char* oldName, const char* newName);
	int MkDir(const char* strDirName, unsigned nMode = 0755);
	CString Join(const char* dirname, const char* basename);
	list<CString> FileInDir(const char* strDirName);
}
