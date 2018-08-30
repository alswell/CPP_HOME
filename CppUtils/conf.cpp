#include "conf.h"
#include "file.h"


CConf::CConf(const char* filename)
{
	CString strSection;
	CFile file(filename/*, "rb"*/);
	list<CString> ls = file.ReadLines();
	FOR_LIST(CString, ls, it)
	{
		it->Trim();
		if (it->GetLength() == 0)
			continue;
		if ((*it)[0] == '[' && (*it)[-1] == ']')
		{
			strSection = it->SubStr(1, -1);
		}
		else if ((*it)[0] != '#')
		{
			list<CString> lsKV = it->Split('=');
			if (lsKV.size() == 2)
				m_kv[strSection][lsKV.front().Trim()] = lsKV.back().Trim();
		}
	}
}

CConf::~CConf()
{
}

DICT(CString) &CConf::operator [](CString section)
{
	return m_kv[section];
}
