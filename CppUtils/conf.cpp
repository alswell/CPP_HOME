#include "conf.h"
#include "file.h"


CConf::CConf(const char* filename)
{
	CStr strSection;
	CFile file(filename/*, "rb"*/);
	list<CStr> ls = file.ReadLines();
	FOR_LIST(CStr, ls, it)
	{
		it->Trim();
		if (it->Length() == 0)
			continue;
		if ((*it)[0] == '[' && (*it)[-1] == ']')
		{
			strSection = it->SubStr(1, -1);
		}
		else if ((*it)[0] != '#')
		{
			list<CStr> lsKV = it->Split('=');
			if (lsKV.size() == 2)
				m_kv[strSection][lsKV.front().Trim()] = lsKV.back().Trim();
		}
	}
}

CConf::~CConf()
{
}

DICT(CStr) &CConf::operator [](CStr section)
{
	return m_kv[section];
}
