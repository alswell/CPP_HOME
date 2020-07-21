#pragma once
#include "env.h"

bool StrCMP_YAML(const char* key, const char* KEY, const char* key0);
int DoParseYaml(int& value, const char*& p, int indent0);
int DoParseYaml(double& value, const char*& p, int indent0);
int DoParseYamlUnknown(const char*& p, int indent0);
int CountIndent(const char*& p);

template <class T>
int DoParseYaml(vector<T>& value, const char*& p, int indent0)
{
	T tmp;
	value.clear();
	if (p == nullptr) return 0;
	int indent = CountIndent(p);
	if (indent < indent0)
		return indent;
	while (true)
	{
		if (p[0] != '-' || (++p)[0] != ' ') throw 20011;
		++p;
		auto r = DoParseYaml(tmp, p, indent);
		value.push_back(tmp);
		if (r != indent)
			return r;
	}
	return 0;
}

#define DEF_YAML_ITEM(type, name)\
	type name;\
	virtual int Parse_##name(const char* key, const char* KEY, const char*& p, int indent)\
	{\
		if (key == nullptr)\
		{\
			DoParseYaml(name, key, 0);\
			return -2;\
		}\
		if (!StrCMP_YAML(key, KEY, #name))\
			return -3;\
		return DoParseYaml(name, p, indent);\
	}
#define END_YAML_ITEM() virtual int ParseEnd(const char*, const char*, const char*&, int) { return -1; }

typedef int(*VF_PARSE_YAML)(void*, const char*, const char*, const char*&, int);
template<class T>
int DoParseYaml(T& value, const char*& p, int indent0)
{
	auto pList = reinterpret_cast<VF_PARSE_YAML*>(*reinterpret_cast<int64_t*>(&value));
	for (int i = 0; pList[i](&value, nullptr, nullptr, p, 0) != -1; ++i);
	if (p == nullptr) return 0;
	int indent = CountIndent(p);
	if (indent <= indent0)
		return indent;

	const char* key = p;
	while (*p)
	{
		if (*p == ':')
		{
			auto KEY = p++;
			int r = 0;
			for (int i = 0; (r = pList[i](&value, key, KEY, p, indent)) == -3 || ((r == -1 && (r = DoParseYamlUnknown(p, indent))) && false); ++i);
			if (r != indent)
				return r;
			key = p;
			continue;
		}
		++p;
	}
	return 0;
}

template<class T>
int ParseYaml(T& value, const char* p)
{
	try {
		DoParseYaml(value, p, -1);
	} catch (int i) {
		return i;
	}
	return 0;
}
