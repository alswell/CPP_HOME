#include "yaml.h"

bool StrCMP_YAML(const char* key, const char* KEY, const char* key0)
{
	while (key != KEY && *key0 && *key == *key0)
	{
		++key;
		++key0;
	}
	return (key == KEY || *key == ' ') && *key0 == 0;
}

const char* DoParseYaml(int& value, const char* p)
{
	char* tmp;
	value = int(strtol(p, &tmp, 10));
	return tmp;
}

const char* DoParseYaml(double& value, const char* p)
{
	char* tmp;
	value = strtod(p, &tmp);
	return tmp;
}

int DoParseYamlUnknown(const char*& p, int indent)
{
	while (*p)
	{
		if (*p == '\n')
		{
			auto r = CountIndent(p);
			if (r <= indent)
				return r;
			continue;
		}
		++p;
	}
	return -1;
}

int CountIndent(const char*& p)
{
	int indent = 0;
	while (*p)
	{
		switch (*p)
		{
		case '#':
			while (*p && *p != '\n') ++p;
			if (*p == 0)
				return -1;
		case '\r':
		case '\n':
			indent = 0;
			break;
		case ' ':
			++indent;
			break;
		default:
			return indent;
		}
		++p;
	}
	return -1;
}

