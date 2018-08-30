#pragma once

#include "env.h"
#include "str.h"
#include "dict.h"

class CConf
{
	DICT(DICT(CString)) m_kv;
public:
	CConf(const char *filename);
	~CConf();

	DICT(CString) & operator [] (CString section);
};

