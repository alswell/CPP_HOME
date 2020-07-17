#include "json.h"

bool StrCMP(const char* key, const char* KEY, const char* key0)
{
	while (key != KEY && *key0 && *key == *key0)
	{
		++key;
		++key0;
	}
	return key == KEY && *key0 == 0;
}

const char* DoParseJson(bool& value, const char* p)
{
	value = false;
	if (p == nullptr) return nullptr;
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

const char* DoParseJson(int& value, const char* p)
{
	char* tmp = nullptr;
	value = p ? int(strtol(p, &tmp, 10)) : 0;
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

const char* DoParseJson(double& value, const char* p)
{
	char* tmp = nullptr;
	value = p ? strtod(p, &tmp) : 0;
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

const char* DoParseJson(const char*& value, const char* p)
{
	value = nullptr;
	if (p == nullptr) return nullptr;
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

const char* DoParseJsonDict(const char* p)
{
	while (*p)
	{
		switch (*p)
		{
		case '[':
			p = DoParseJsonList(p + 1);
			break;
		case '{':
			p = DoParseJsonDict(p + 1);
			break;
		case '}':
			return p;
		}
		++p;
	}
	return p;
}

const char* DoParseJsonList(const char* p)
{
	while (*p)
	{
		switch (*p)
		{
		case '[':
			p = DoParseJsonList(p + 1);
			break;
		case '{':
			p = DoParseJsonDict(p + 1);
			break;
		case ']':
			return p;
		}
		++p;
	}
	return p;
}

const char* DoParseJsonUnknown(const char* p)
{
	while (*p)
	{
		switch (*p)
		{
		case '[':
			p = DoParseJsonList(p + 1);
			return p + 1;
		case '{':
			p = DoParseJsonDict(p + 1);
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


void DoDumpJson(bool value, CString &buff, int)
{
	buff += (value ? "true" : "false");
}

void DoDumpJson(int value, CString& buff, int)
{
	buff.AppendFormat("%d", value);
}

void DoDumpJson(double value, CString& buff, int)
{
	buff.AppendFormat("%f", value);
}

void DoDumpJson(const char *value, CString& buff, int)
{
	buff += (value ? value : "null");
}
