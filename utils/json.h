#pragma once
#include "../env.h"
#include "../str.h"


bool StrCMP_JSON(const char* key, const char* KEY, const char* key0);
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
	if (p == nullptr) return nullptr;\
	while (*p && (*p == ' ' || *p == '\t' || *p == '\n')) ++p;\
	if (*p == 0) throw 10011;\
	if (*p != '[') throw 20011;\
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
	if (*p == 0) throw 10012;\
	return p;\
}
PARSE_JSON_LIST(list)
PARSE_JSON_LIST(vector)

void DoDumpJson(bool value, CString& buff, int indent);
void DoDumpJson(int value, CString& buff, int indent);
void DoDumpJson(double value, CString& buff, int indent = 0);
void DoDumpJson(const char* value, CString& buff, int indent = 0);

struct SDumpJson
{
	CString* buff;
	template<class T>
	SDumpJson& operator , (T x)
	{
		DoDumpJson(x, *buff);
		return *this;
	}
};
#define JSON_SEQ(str, ...) SDumpJson{&str}, __VA_ARGS__

#define DUMP_JSON_LIST(type)\
template <class T>\
void DoDumpJson(type<T>& value, CString& buff, int indent)\
{\
	DoDumpJson("[\n", buff);\
	for (auto it = value.begin(); it != value.end(); ++it)\
	{\
		if (it != value.begin())\
			DoDumpJson(",\n", buff);\
		for (int i = 0; i < indent; ++i)\
			DoDumpJson(" ", buff);\
		DoDumpJson(*it, buff, indent);\
	}\
	DoDumpJson("\n", buff);\
	for (int i = 2; i < indent; ++i)\
		DoDumpJson(" ", buff);\
	DoDumpJson("]", buff);\
}
DUMP_JSON_LIST(list)
DUMP_JSON_LIST(vector)


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
			DoParseJson(name, nullptr);\
			return 2;\
		}\
		if (!StrCMP_JSON(key, KEY, #name))\
			return 0;\
		p = DoParseJson(name, p+1);\
		return 1;\
	}\
	virtual int Dump_##name(CString& buff, int indent, bool first) {\
		if (!first)\
			DoDumpJson(",\n", buff);\
		for (int i = 0; i < indent; ++i)\
			DoDumpJson(" ", buff);\
		DoDumpJson("\""#name"\": ", buff);\
		DoDumpJson(name, buff, indent+2);\
		return 0;\
	}

#define END_JSON_ITEM()\
	virtual int ParseEnd(const char*, const char*, const char*&) { return -1; }\
	virtual int DumpEnd(int) { return -1; }

typedef int(*VF_PARSE_JSON)(void*, const char*, const char*, const char*&);
template<class T>
const char* DoParseJson(T& value, const char* p)
{
	auto pList = reinterpret_cast<VF_PARSE_JSON*>(*reinterpret_cast<int64_t*>(&value));
	for (int i = 0; pList[i](&value, nullptr, nullptr, p) == 2; i+=2);
	if (p == nullptr) return nullptr;

	while (*p && (*p == ' ' || *p == '\t' || *p == '\n')) ++p;
	if (*p == 0) throw 10001;
	if (*p != '{') throw 20001;
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
			for (int i = 0, r = 0; (r = pList[i](&value, key, KEY, p)) == 0 || (r != 1 && (p = DoParseJsonUnknown(p + 1)) && false); i+=2);
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

typedef int(*VF_DUMP_JSON)(void*, CString&, int, bool);
template<class T>
const char* DoDumpJson(T& value, CString& buff, int indent)
{
	auto pList = reinterpret_cast<VF_DUMP_JSON*>(*reinterpret_cast<int64_t*>(&value)) + 1;
	DoDumpJson("{\n", buff);
	for (int i = 0; pList[i](&value, buff, indent+2, i == 0) == 0; i+=2);
	DoDumpJson("\n", buff);
	for (int i = 0; i < indent; ++i)
		DoDumpJson(" ", buff);
	DoDumpJson("}", buff);
}

template<class T>
void DumpJson(T& value, CString& buff)
{
	buff = "";
	DoDumpJson(value, buff, 0);
}
