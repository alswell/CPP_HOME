#ifndef __CONF_H
#define __CONF_H

#include "afx.h"
#include "str.h"

#define DICT(value_type) map<CStr, value_type>
//#include <hash_map>
//#define DICT(value_type) __gnu_cxx::hash_map<const char*, value_type>

class CConf
{
	DICT(DICT(CStr)) m_kv;
public:
	CConf(CStr strFileName);
	~CConf();

	CStr& operator () (CStr section, CStr key);
	DICT(CStr) & operator [] (CStr section);
};

#endif
