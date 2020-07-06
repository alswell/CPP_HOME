#pragma once
#include "env.h"

//namespace JSON {
//	CString Dump(const DICT(CString)& dict);
//}

bool StrCMP(const char* key, const char* KEY, const char* key0);
const char* ParseJson(bool& value, const char* p);
const char* ParseJson(int& value, const char* p);
const char* ParseJson(double& value, const char* p);
const char* ParseJson(const char*& value, const char* p);
const char* ParseDict(const char* p);
const char* ParseList(const char* p);
const char* ParseUnknown(const char* p);

#define PARSE_JSON_LIST(type) \
template <class T>\
const char* ParseJson(type<T>& value, const char* p) {\
	T tmp;\
	value.clear();\
	while (*p)\
	{\
		switch (*p)\
		{\
			case ',':\
			case '[':\
				p = ParseJson(tmp, p + 1);\
				value.push_back(tmp);\
				break;\
			case ']':\
				return p;\
			default:\
				++p;\
		}\
	}\
	return p;\
}
PARSE_JSON_LIST(list)
PARSE_JSON_LIST(vector)

#define PARSE(name) \
if (StrCMP(key, KEY, #name))\
{\
	p = ParseJson(value.name, p + 1);\
	break;\
}

#define PARSE_STRUCT_BEGIN(cls)\
const char* ParseJson(cls& value, const char* p)\
{\
	memset(&value, 0, sizeof(cls));\
	while (*p != '{') ++p;\
	const char* key = nullptr;\
	const char* KEY = nullptr;\
	while (*p)\
	{\
		switch (*p)\
		{\
		case '}':\
			return p;\
		case '"':\
			if (key == nullptr)\
			{\
				key = p + 1;\
				break;\
			}\
			KEY = p;\
			break;\
		case ':':\
			do\
			{

#define PARSE_STRUCT_END()\
				p = ParseUnknown(p + 1);\
				while (*p)\
				{\
					if (*p == ',' || *p == '}')\
						break;\
					++p;\
				}\
			} while (false);\
			key = nullptr;\
			if (*p == '}')\
				return p;\
			break;\
		}\
		++p;\
	}\
	return p;\
}

#define DEF_JSON_ITEM(type, name)\
	type name;\
	virtual int Parse_##name(const char* key, const char* KEY, const char*& p)\
	{\
		if (key == nullptr)\
		{\
			ParseJson(name, "");\
			return 2;\
		}\
		if (!StrCMP(key, KEY, #name))\
			return 0;\
		p = ParseJson(name, p+1);\
		return 1;\
	}
#define END_JSON_ITEM() virtual int ParseEnd(const char*, const char*, const char*&) { return -1; }

typedef int(*VF)(void*, const char*, const char*, const char*&);
template<class T>
const char* ParseJson(T& value, const char* p)
{
	auto pList = reinterpret_cast<VF*>(*reinterpret_cast<int64_t*>(&value));
	for (int i = 0; pList[i](&value, nullptr, nullptr, p) == 2; ++i);

	while (*p != '{') ++p;
	const char* key = nullptr;
	const char* KEY = nullptr;
	while (*p)
	{
		switch (*p)
		{
		case '}':
			return p;
		case '"':
			if (key == nullptr)
			{
				key = p + 1;
				break;
			}
			KEY = p;
			break;
		case ':':
			do
			{
				int i = 0, r = 0;
				while ((r = pList[i++](&value, key, KEY, p)) == 0);
				if (r == 1)
					break;
				p = ParseUnknown(p + 1);
				while (*p)
				{
					if (*p == ',' || *p == '}')
						break;
					++p;
				}
			} while (false);
			key = nullptr;
			if (*p == '}')
				return p;
			break;
		}
		++p;
	}
	return p;
}

