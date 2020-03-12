#include "path.h"
#include <sys/stat.h>
#include <dirent.h>


CString PATH::Prefix(const CString &strFileName)
{
	return strFileName.Left(unsigned(strFileName.ReverseFind('.')));
}

CString PATH::Suffix(const CString &strFileName)
{
	return strFileName.Mid(strFileName.ReverseFind('.'));
}

CString PATH::Basename(const CString &strFullPath)
{
	return strFullPath.Mid(strFullPath.ReverseFind('/') + 1);
}

CString PATH::Dirname(const CString &strFullPath)
{
	return strFullPath.Left(unsigned(strFullPath.ReverseFind('/')) + 1);
}

int PATH::Rename(const char* oldName, const char* newName)
{
	return rename(oldName, newName);
}

int PATH::MkDir(const char *strDirName, unsigned nMode)
{
	return mkdir(strDirName, nMode);
}

CString PATH::Join(const char* dirname, const char* basename)
{
	CString path(dirname);
	if (path[-1] != '/')
		path += '/';
	path += basename;
	return path;
}

list<CString> PATH::FileInDir(const char *strDirName)
{
	list<CString> lsFile;
	DIR* pDir = opendir(strDirName);
	if (pDir)
	{
		struct dirent *dp;
		while ((dp = readdir(pDir)) != nullptr)
		{
			if (dp->d_name[0] == '.')
				continue;
			//cout << dp->d_name << endl;
			lsFile.push_back(dp->d_name);
		}
		closedir(pDir);
		lsFile.sort();
	}
	return lsFile;
}
