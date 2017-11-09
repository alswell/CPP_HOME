#ifndef __ARGS_H
#define __ARGS_H
#include "afx.h"

#include "dict.h"
#include "str.h"

class ArgsParser
{
	DICT(CStr) m_kv;
	DICT(ArgsParser*) m_subs;
public:
	ArgsParser(char **argv);

	CStr Get(const CStr& key, const char* strDefault = NULL);

	CStr &operator [] (const CStr& key);
	ArgsParser &operator () (const CStr& sub);

	void _str();
};


#endif
