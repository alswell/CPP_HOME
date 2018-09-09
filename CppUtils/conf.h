#pragma once

#include "env.h"
#include "str.h"
#include "dict.h"

class CConf
{
	static CString m_strDefaultFile;
	static CConf* m_pStatic;

	CString m_strFileName;
	list<CString> m_lsSection;
	DICT(DICT(CString)) m_kv;
public:
	CConf(const char *filename);
	~CConf();
	static void SetDefault(const char* strFileName);
	static CConf &Instance();

	DICT(CString) & operator [] (CString section);
	void Save();
};

