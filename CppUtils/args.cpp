#include "args.h"


ArgsParser::ArgsParser(char **argv)
{
	CStr strCurKey = "";
	int i = 1;
	while (1)
	{
		char* p = argv[i];
		if (p == NULL)
			break;

		CStr str(p);
		if (str.StartWith("--"))
		{
			if (strCurKey != "")
			{
				m_kv[strCurKey] = "true";
				strCurKey = "";
			}
			list<CStr> lsKV = str.Right(2).Split('=');
			if (lsKV.size() == 2)
				m_kv[lsKV.front()] = lsKV.back();
			else
				strCurKey = lsKV.front();
		}
		else
		{
			if (strCurKey != "")
			{
				m_kv[strCurKey] = str;
				strCurKey = "";
			}
			else
			{
				m_subs[str] = new ArgsParser(&argv[i]);
				return;
			}
		}

		++i;
	}
}

CStr ArgsParser::Get(const CStr &key, const char *strDefault)
{
	CStr& strValue = (*this)[key];
	if (strValue != "")
		return strValue;
	if (strDefault)
		return CStr(strDefault);
	return "";
}

CStr &ArgsParser::operator [](const CStr &key)
{
	return m_kv[key];
}

ArgsParser &ArgsParser::operator ()(const CStr &sub)
{
	return *m_subs[sub];
}

void ArgsParser::_str()
{
	FOR_DICT(CStr, m_kv, it)
	{
		cout << it->first << ": " << it->second << endl;
	}
	FOR_DICT(ArgsParser*, m_subs, it)
	{
		cout << it->first << endl;
		it->second->_str();
	}
}

