#pragma once
#include "env.h"

bool StrCMP_YAML(const char* key, const char* KEY, const char* key0);
const char* DoParseYaml(int& value, const char* p);
const char* DoParseYaml(double& value, const char* p);
int DoParseYamlUnknown(const char*& p, int indent);
int CountIndent(const char*& p);

#define DEF_YAML_ITEM(type, name)\
	type name;\
	virtual int Parse_##name(const char* key, const char* KEY, const char*& p)\
	{\
		if (key == nullptr)\
		{\
			DoParseYaml(name, "");\
			return 2;\
		}\
		if (!StrCMP_YAML(key, KEY, #name))\
			return 0;\
		p = DoParseYaml(name, p+1);\
		return 1;\
	}
#define END_YAML_ITEM() virtual int ParseEnd(const char*, const char*, const char*&) { return -1; }

typedef int(*VF_PARSE_YAML)(void*, const char*, const char*, const char*&);
template<class T>
const char* DoParseYaml(T& value, const char* p)
{
	int indent = CountIndent(p);
	auto pList = reinterpret_cast<VF_PARSE_YAML*>(*reinterpret_cast<int64_t*>(&value));
	for (int i = 0; pList[i](&value, nullptr, nullptr, p) == 2; ++i);

	const char* key = p;
	while (*p)
	{
		if (*p == ':')
		{
			int ind = 0;
			for (int i = 0, r = 0; (r = pList[i](&value, key, p, p)) == 0 || (ind = r == 1 ? CountIndent(p) : DoParseYamlUnknown(p, indent) && false); ++i);
			if (ind != indent)
				return p;
			key = p;
			continue;
		}
		++p;
	}
	return p;
}

template<class T>
int ParseYaml(T& value, const char* p)
{
	try {
		DoParseYaml(value, p);
	} catch (int i) {
		return i;
	}
	return 0;
}
