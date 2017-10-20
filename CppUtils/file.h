#ifndef __COMMON_FILE_H
#define __COMMON_FILE_H

#include "str.h"

class File
{
	FILE* pf;
	unsigned nSize;
	CStr strBuff;
public:
	File(CStr strFileName, char mode[]);
	~File();

	list<CStr> readlines();
};

#endif
