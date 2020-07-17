#include "ioutil.h"


bool ReadFile(CString& str, const char* filename)
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

bool WriteFile(CString &str, const char *filename)
{
	FILE* pf = fopen(filename, "w");
	if (pf == nullptr)
		return false;
	auto nSize = str.GetLength();
	auto r = fwrite(str.GetString(), 1, size_t(nSize), pf);
	fclose(pf);
	return r == nSize;
}
