#pragma once
#include "env.h"
#include "str.h"

namespace PATH {
	CString Prefix(const CString& strFileName);
	CString Suffix(const CString& strFileName);
	CString FileName(const CString& strFullPath);
	CString PathName(const CString& strFullPath);
	int MkDir(const char* strDirName, unsigned nMode = 0755);
	list<CString> FileInDir(const char* strDirName);
}
