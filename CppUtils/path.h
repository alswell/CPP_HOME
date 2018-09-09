#pragma once
#include "str.h"

namespace PATH {
	CString Suffix(const CString& strFileName);
	CString FileName(const CString& strFullPath);
	CString PathName(const CString& strFullPath);
	int MkDir(const char* strDirName, unsigned nMode = 0755);
}

