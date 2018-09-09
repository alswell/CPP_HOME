#include "path.h"
#include <sys/stat.h>


CString PATH::Suffix(const CString &strFileName)
{
	return strFileName.Mid(strFileName.ReverseFind('.'));
}

CString PATH::FileName(const CString &strFullPath)
{
	return strFullPath.Mid(strFullPath.ReverseFind('/') + 1);
}

CString PATH::PathName(const CString &strFullPath)
{
	return strFullPath.Left(strFullPath.ReverseFind('/') + 1);
}

int PATH::MkDir(const char *strDirName, unsigned nMode)
{
	mkdir(strDirName, nMode);
}


