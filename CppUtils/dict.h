#ifndef __DICT_H
#define __DICT_H

#include <hash_map>
#include <hash_fun.h>
#include "str.h"
struct _str_hash
{
	__gnu_cxx::hash<const char*> _hash;
	size_t operator () (CStr str) const
	{
		return _hash((const char*)str);
	}
};
#define DICT(value_type) __gnu_cxx::hash_map<CStr, value_type, _str_hash>
//#define DICT(value_type) map<CStr, value_type>
#define FOR_DICT(value_type, obj, it) \
for (DICT(value_type)::iterator it = obj.begin(); it != obj.end(); ++it)

#define PRINT_DICT(d) \
for (DICT(int)::iterator it = d.begin(); it != d.end(); ++it)\
	cout << it->first << ": " << it->second << endl;

#endif
