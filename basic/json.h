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
