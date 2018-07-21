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

	CStr& operator () (CStr section, CStr key);
	DICT(CStr) & operator [] (CStr section);
};

