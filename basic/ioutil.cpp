#include "ioutil.h"


bool ReadAll(CString& str, const char* filename)
{
	FILE* pf = fopen(filename, "r");
	if (pf == nullptr)
		return false;
	fseek(pf, 0, SEEK_END);
	auto nSize = ftell(pf);
	if (nSize == -1)
		return false;
	rewind(pf);
	auto buff = str.GetBuffer(unsigned(nSize + 1));
	auto r = fread(buff, 1, size_t(nSize), pf);
	buff[r] = 0;
	str.ReleaseBuffer();
	fclose(pf);
	return true;
}
