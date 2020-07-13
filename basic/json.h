#pragma once
#include "env.h"

//namespace JSON {
//	CString Dump(const DICT(CString)& dict);
//}

bool StrCMP(const char* key, const char* KEY, const char* key0);
const char* DoParseJson(bool& value, const char* p);
const char* DoParseJson(int& value, const char* p);
const char* DoParseJson(double& value, const char* p);
const char* DoParseJson(const char*& value, const char* p);
const char* DoParseJsonDict(const char* p);
const char* DoParseJsonList(const char* p);
const char* DoParseJsonUnknown(const char* p);

#define PARSE_JSON_LIST(type) \
template <class T>\
const char* DoParseJson(type<T>& value, const char* p) {\
	T tmp;\
	value.clear();\
	while (*p)\
	{\
		switch (*p)\
		{\
			case ',':\
			case '[':\
				p = DoParseJson(tmp, p + 1);\
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
	p = DoParseJson(value.name, p + 1);\
	break;\
}

#define PARSE_STRUCT_BEGIN(cls)\
const char* DoParseJson(cls& value, const char* p)\
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
			DoParseJson(name, "");\
			return 2;\
		}\
		if (!StrCMP(key, KEY, #name))\
			return 0;\
		p = DoParseJson(name, p+1);\
		return 1;\
	}
#define END_JSON_ITEM() virtual int ParseEnd(const char*, const char*, const char*&) { return -1; }

typedef int(*VF)(void*, const char*, const char*, const char*&);
template<class T>
const char* DoParseJson(T& value, const char* p)
{
	auto pList = reinterpret_cast<VF*>(*reinterpret_cast<int64_t*>(&value));
	for (int i = 0; pList[i](&value, nullptr, nullptr, p) == 2; ++i);

	while (*p && *p != '{') ++p;
	if (*p == 0) throw 10001;
	const char* key = nullptr;
	const char* KEY = nullptr;
	while (*p)
	{
		switch (*p)
		{
		case '}':
			return p;
		case ':':
			if (KEY == nullptr) throw 1;
			for (int i = 0, r = 0; (r = pList[i](&value, key, KEY, p)) == 0 || (r != 1 && (p = DoParseJsonUnknown(p + 1)) && false); ++i);
			KEY = key = nullptr;
			break;
		case '"':
			if (key == nullptr)
				key = p + 1;
			else
				KEY = p;
		default:
			++p;
		}
	}
	if (*p == 0) throw 10002;
	return p;
}

template<class T>
int ParseJson(T& value, const char* p)
{
	try {
		DoParseJson(value, p);
	} catch (int i) {
		return i;
	}
	return 0;
}
