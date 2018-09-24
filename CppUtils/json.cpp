#include "json.h"

CString JSON::Dump(const DICT(CString)& dict)
{
	CString str = "{";
	FOR_DICT_CONST(CString, dict, it)
	{
		str.AppendFormat("\"%s\": \"%s\", ", (const char*)it->first, (const char*)it->second);
	}
	str.TrimRight()[-1] = '}';
	return str;
}

void JSON::LoadFile(CSmartType &json, const char *filename)
{
	FILE* pf = fopen(filename, "r");
	fseek(pf, 0, SEEK_END);
	unsigned nSize = ftell(pf);
	rewind(pf);
	char* buff = new char[nSize + 1];
	int r = fread(buff, 1, nSize, pf);
	fclose(pf);
	if (r < 0)
	{
		delete[] buff;
		return;
	}
	buff[r] = 0;
	cout << buff << endl;
	Load(json, buff);
	delete[] buff;
}

void JSON::Load(CSmartType &json, const char *str)
{
	char* p = (char*)str;

	while (*p)
	{
		switch (*p)
		{
		case '{':
			p = JSON::ParseDict(json, p);
			break;
		case '[':
			p = JSON::ParseList(json, p);
			break;
		}
		++p;
	}
}

char* JSON::ParseDict(CSmartType &dict, const char *str)
{
	char* p = (char*)str + 1;
	char* key = p;
	char* value;
	dict.ToDict();
	map<CString, CSmartType>& D = dict;
	CString tmp_key;

	while (*p)
	{
		switch (*p)
		{
		case ':':
			value = p+1;
			break;
		case ',':
		case '}':
			if (key)
			{
				tmp_key = CString(key, value-key-1).Trim();
				D[tmp_key.SubStr(1, -1)].SmartInit(CString(value, p-value).Trim());
				//if (tmp_key[0] == '"')
				//	D[tmp_key.SubStr(1, -1)].SmartInit(CString(value, p-value).Trim());
				//else
				//	D[atoi((const char*)tmp_key)].SmartInit(CString(value, p-value).Trim());
			}
			if (*p == '}')
				break;
			key = p+1;
			break;
		case '{':
			tmp_key = CString(key, value-key-1).Trim();
			p = JSON::ParseDict(D[tmp_key.SubStr(1, -1)], p);
			//if (tmp_key[0] == '"')
			//	p = JSON::ParseDict(D[tmp_key.SubStr(1, -1)], p);
			//else
			//	p = JSON::ParseDict(D[atoi((const char*)tmp_key)], p);
			key = NULL;
			break;
		case '[':
			tmp_key = CString(key, value-key-1).Trim();
			p = JSON::ParseList(D[tmp_key.SubStr(1, -1)], p);
			//if (tmp_key[0] == '"')
			//	p = JSON::ParseList(D[tmp_key.SubStr(1, -1)], p);
			//else
			//	p = JSON::ParseList(D[atoi((const char*)tmp_key)], p);
			key = NULL;
			break;
		}
		++p;
	}
	return p;
}

char *JSON::ParseList(CSmartType &list, const char *str)
{
	char* p = (char*)str + 1;
	char* key = p;
	list.ToList();
	vector<CSmartType>& L = list;

	while (*p)
	{
		switch (*p)
		{
		case ',':
		case ']':
			if (key)
			{
				L.push_back(CSmartType());
				L.back().SmartInit(CString(key, p-key).Trim());
			}
			if (*p == ']')
				break;
			key = p+1;
			break;
		case '[':
			L.push_back(CSmartType());
			p = JSON::ParseList(L.back(), p);
			key = NULL;
			break;
		case '{':
			L.push_back(CSmartType());
			p = JSON::ParseDict(L.back(), p);
			key = NULL;
			break;
		}
		++p;
	}
	return p;
}
