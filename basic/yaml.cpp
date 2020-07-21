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

int DoParseYaml(int& value, const char *&p, int indent0)
{
	char* tmp = nullptr;
	value = p ? int(strtol(p, &tmp, 10)) : 0;
	p = tmp;
	return CountIndent(p);
}

int DoParseYaml(double& value, const char *&p, int indent0)
{
	char* tmp = nullptr;
	value = p ? strtod(p, &tmp) : 0;
	p = tmp;
	return CountIndent(p);
}

int DoParseYamlUnknown(const char*& p, int indent0)
{
	while (*p)
	{
		if (*p == '\n')
		{
			auto indent = CountIndent(p);
			if (indent <= indent0)
				return indent;
			continue;
		}
		++p;
	}
	return -1;
}

int CountIndent(const char*& p)
{
	if (p == nullptr) return -2;
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

