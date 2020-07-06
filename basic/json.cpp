#include "json.h"

//CString JSON::Dump(const DICT(CString)& dict)
//{
//	CString str = "{";
//	FOR_DICT_CONST(CString, dict, it)
//	{
//		str.AppendFormat("\"%s\": \"%s\", ", (const char*)it->first, (const char*)it->second);
//	}
//	str.TrimRight()[-1] = '}';
//	return str;
//}

bool StrCMP(const char* key, const char* KEY, const char* key0)
{
	while (key != KEY && *key0 && *key == *key0)
	{
		++key;
		++key0;
	}
	return key == KEY && *key0 == 0;
}

const char* ParseJson(bool& value, const char* p)
{
	while (*p)
	{
		switch (*p)
		{
		case ' ':
		case '\r':
		case '\n':
		case '\t':
			break;
		case 't':
			value = true;
			p += 2;
			break;
		case 'f':
			value = false;
			p += 3;
			break;
		case ',':
		case '}':
			return p;
		}
		++p;
	}
	return p;
}

const char* ParseJson(int& value, const char* p)
{
	char* tmp;
	value = int(strtol(p, &tmp, 10));
	return tmp;
//	value = atoi(p);
//	while (*p)
//	{
//		switch (*p)
//		{
//		case ',':
//		case '}':
//			return p;
//		}
//		++p;
//	}
//	return p;
}

const char* ParseJson(double& value, const char* p)
{
	char* tmp;
	value = strtod(p, &tmp);
	return tmp;
//	value = atof(p);
//	while (*p)
//	{
//		switch (*p)
//		{
//		case ',':
//		case '}':
//			return p;
//		}
//		++p;
//	}
//	return p;
}

const char* ParseJson(const char*& value, const char* p)
{
	value = nullptr;
	const char* str = nullptr;
	while (*p)
	{
		switch (*p)
		{
		case '"':
			if (str == nullptr)
			{
				str = p + 1;
			}
			else
			{
				auto len = p - str;
				auto tmp = new char[len + 1];
				memcpy(tmp, str, size_t(len));
				tmp[len] = 0;
				value = tmp;
				str = nullptr;
			}
			break;
		case ',':
		case '}':
			if (str == nullptr)
				return p;
		}
		++p;
	}
	return p;
}

const char* ParseDict(const char* p)
{
	while (*p)
	{
		switch (*p)
		{
		case '[':
			p = ParseList(p + 1);
			break;
		case '{':
			p = ParseDict(p + 1);
			break;
		case '}':
			return p;
		}
		++p;
	}
	return p;
}

const char* ParseList(const char* p)
{
	while (*p)
	{
		switch (*p)
		{
		case '[':
			p = ParseList(p + 1);
			break;
		case '{':
			p = ParseDict(p + 1);
			break;
		case ']':
			return p;
		}
		++p;
	}
	return p;
}

const char* ParseUnknown(const char* p)
{
	while (*p)
	{
		switch (*p)
		{
		case '[':
			p = ParseList(p + 1);
			return p + 1;
		case '{':
			p = ParseDict(p + 1);
			return p + 1;
		case ',':
		case '}':
		case ']':
			return p;
		}
		++p;
	}
	return p;
}

