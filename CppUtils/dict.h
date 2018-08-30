#pragma once
#include <hash_map>
#include <hash_fun.h>
#include "fmt.h"
struct _str_hash
{
	__gnu_cxx::hash<const char*> _hash;
	size_t operator () (CString str) const;
};
#define DICT(value_type) typename::__gnu_cxx::hash_map<CString, value_type, _str_hash>
//#define DICT(value_type) map<CString, value_type>

#define FOR_DICT(value_type, obj, it) \
for (DICT(value_type)::iterator it = obj.begin(); it != obj.end(); ++it)

#define PRINT_DICT(d) \
for (DICT(int)::iterator it = d.begin(); it != d.end(); ++it)\
	cout << it->first << ": " << it->second << endl;

#define FOR_HASH(key_type, value_type, obj, it) \
for (typename::__gnu_cxx::hash_map<key_type, value_type>::iterator it = obj.begin(); it != obj.end(); ++it)

template <class T>
class CDict
{
public:
	DICT(T) m_strDict;
	__gnu_cxx::hash_map<int, T> m_intDict;
public:
	T &operator [] (int i)
	{
		return m_intDict[i];
	}

	T &operator [] (const CString& str)
	{
		return m_strDict[str];
	}

	CDict<T>& operator = (const CDict<T>& d)
	{
		FOR_DICT(T, d.m_strDict, it)
			m_strDict[it->first] = it->second;
		FOR_HASH(int, T, d.m_intDict, it)
			m_intDict[it->first] = it->second;
	}

	CString Str()
	{
		CString str = "{";
		FOR_HASH(int, T, m_intDict, it)
		{
			str.AppendFormat("%d: %s, ", it->first, _(it->second));
		}

		FOR_DICT(T, m_strDict, it)
		{
			str.AppendFormat("'%s': %s, ", (const char*)it->first, _(it->second));
		}
		str += "}";
		return str;
	}
};

