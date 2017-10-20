#include "file.h"


File::File(CStr strFileName, char mode[])
{
	pf = fopen(strFileName, mode);
	fseek(pf, 0, SEEK_END);
	nSize = ftell(pf);
	rewind(pf);
	fread(strBuff.GetBuffer(nSize + 1), 1, nSize, pf);
	strBuff.ReleaseBuffer();
}


File::~File()
{
	fclose(pf);
}

list<CStr> File::readlines()
{
	return strBuff.Split('\n');
}

