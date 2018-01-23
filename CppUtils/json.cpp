#include "json.h"

void JSON::Dump(CStr& str, DICT(CStr) dict)
{
	str = "{";
	FOR_DICT(CStr, dict, it)
	{
		str.AppendFormat("\"%s\": \"%s\", ", (const char*)it->first, (const char*)it->second);
	}
	str.TrimRight()[-1] = '}';
}

void JSON::Load(CValueType &json, const char *str)
{
	char* p = (char*)str;

	while (*p)
	{
		switch (*p)
		{
		case '{':
			json.SetDict();
			p = JSON::ParseDict(json, p+1);
			break;
		case '[':
			json.SetList();
			p = JSON::ParseList(json, p+1);
			break;
		}
		++p;
	}
}

char* JSON::ParseDict(CValueType &dict, const char *str)
{
	char* p = (char*)str;
	char* key = p;
	char* value;
	CDict<CValueType>& D = dict.GetDict();
	CStr tmp_key;

	while (*p)
	{
		switch (*p)
		{
		case ':':
			value = p+1;
			break;
		case ',':
			tmp_key = CStr(key, value-key-1).Trim();
			if (tmp_key[0] == '"')
			{
				tmp_key = tmp_key.SubStr(1, -1);
				if (D[tmp_key].m_type == TYPE_UNKNOWN)
					D[tmp_key].SetStrInt(CStr(value, p-value).Trim());
			}
			else
			{
				int i = atoi((const char*)tmp_key);
				if (D[i].m_type == TYPE_UNKNOWN)
					D[i].SetStrInt(CStr(value, p-value).Trim());
			}
			key = p+1;
			break;
		case '}':
			tmp_key = CStr(key, value-key-1).Trim();
			if (tmp_key[0] == '"')
			{
				tmp_key = tmp_key.SubStr(1, -1);
				if (D[tmp_key].m_type == TYPE_UNKNOWN)
					D[tmp_key].SetStrInt(CStr(value, p-value).Trim());
			}
			else
			{
				int i = atoi((const char*)tmp_key);
				if (D[i].m_type == TYPE_UNKNOWN)
					D[i].SetStrInt(CStr(value, p-value).Trim());
			}
			return p;
			break;
		case '{':
			tmp_key = CStr(key, value-key-1).Trim();
			if (tmp_key[0] == '"')
			{
				CValueType& d = D[tmp_key.SubStr(1, -1)];
				d.SetDict();
				p = JSON::ParseDict(d, p+1);
			}
			else
			{
				CValueType& d = D[atoi((const char*)tmp_key)];
				d.SetDict();
				p = JSON::ParseDict(d, p+1);
			}
			break;
		case '[':
			tmp_key = CStr(key, value-key-1).Trim();
			if (tmp_key[0] == '"')
			{
				CValueType& d = D[tmp_key.SubStr(1, -1)];
				d.SetList();
				p = JSON::ParseList(d, p+1);
			}
			else
			{
				CValueType& d = D[atoi((const char*)tmp_key)];
				d.SetList();
				p = JSON::ParseList(d, p+1);
			}
			break;
		}
		++p;
	}
	return --p;
}

char *JSON::ParseList(CValueType &list, const char *str)
{
	char* p = (char*)str;
	char* key = p;
	vector<CValueType>& L = list.GetList();

	while (*p)
	{
		switch (*p)
		{
		case ',':
			if (key)
			{
				L.push_back(CValueType());
				L.back().SetStrInt(CStr(key, p-key).Trim());
			}
			key = p+1;
			break;
		case ']':
			if (key)
			{
				L.push_back(CValueType());
				L.back().SetStrInt(CStr(key, p-key).Trim());
			}
			return p;
			break;
		case '[':
			L.push_back(CValueType());
			L.back().SetList();
			p = JSON::ParseList(L.back(), p+1);
			key = NULL;
			break;
		case '{':
			L.push_back(CValueType());
			L.back().SetDict();
			p = JSON::ParseDict(L.back(), p+1);
			key = NULL;
			break;
		}
		++p;
	}
	return --p;
}
