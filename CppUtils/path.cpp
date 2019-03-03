#include "path.h"
#include <sys/stat.h>
#include <dirent.h>


CString PATH::Prefix(const CString &strFileName)
{
	return strFileName.Left(strFileName.ReverseFind('.'));
}

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
	return mkdir(strDirName, nMode);
}

list<CString> PATH::FileInDir(const char *strDirName)
{
	list<CString> lsFile;
	DIR* pDir = opendir(strDirName);
	if (pDir)
	{
		struct dirent *dp;
		while ((dp = readdir(pDir)) != NULL)
		{
			if (dp->d_name[0] == '.')
				continue;
			cout << dp->d_name << endl;
			lsFile.push_back(dp->d_name);
		}
		closedir(pDir);
		lsFile.sort();
	}
	return lsFile;
}
