#pragma once

#include "env.h"
#include "str.h"
#include "dict.h"

class CConf
{
	DICT(DICT(CStr)) m_kv;
public:
	CConf(const char *filename);
	~CConf();

	DICT(CStr) & operator [] (CString section);
};

