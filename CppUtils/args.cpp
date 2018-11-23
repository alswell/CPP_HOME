#include "args.h"

#define PCC const char*
int CArgParser::m_nArg;
char** CArgParser::m_pArgs;
CArgParser::CArgParser(const char *sub_cmd, const char *description, CB cb)
{
	m_strName = sub_cmd;
	m_strDescription = description;
	AddOption("help", 'h', ARG_TYPE_BOOL, false, false, "show help message");
	m_pListPosition = NULL;
	m_pSubParser = NULL;
	m_cb = cb;
}

CArgParser::CArgParser(int argc, char *argv[], const char *description)
	: CArgParser(argv[0], description, NULL)
{
	m_nArg = argc;
	m_pArgs = argv;
}

CSmartType GetInitVal(EArgType type, bool is_list)
{
	if (is_list)
		return L0;
	switch (type) {
	case ARG_TYPE_BOOL:
		return false;
		break;
	default:
		return CSmartType();
		break;
	}
}
void CArgParser::AddOption(const char *name, char short_name, EArgType type, bool required, bool is_list, const char *help)
{
	SArgInfo info = {name, short_name, type, required, is_list, help, GetInitVal(type, is_list)};
	m_lsArgInfo.push_back(info);
	map<CString, list<SArgInfo>::iterator>::iterator it;
	it = m_mapKV.find(name);
	if (it != m_mapKV.end())
	{
		cout << "CArgParser:: duplicate key: " << name << endl;
		exit(-1);
	}
	it = m_mapKV.find(short_name);
	if (it != m_mapKV.end())
	{
		cout << "CArgParser:: duplicate key: " << short_name << endl;
		exit(-1);
	}
	m_mapKV[name] = --m_lsArgInfo.end();
	m_mapKV[short_name] = m_mapKV[name];
}

void CArgParser::AddPosition(const char *name, EArgType type, bool is_list, const char *help)
{
	if (m_pListPosition && is_list)
	{
		cout << "Only one positional argument can be defined as list!" << endl;
		exit(-1);
	}
	SArgInfo info = {name, 0, type, true, is_list, help, GetInitVal(type, is_list)};
	m_lsPositional.push_back(info);
	if (is_list)
		m_pListPosition = &m_lsPositional.back();
}

CArgParser &CArgParser::AddSub(const char* sub_cmd, CB cb, const char* description)
{
	CArgParser* pSub = new CArgParser(sub_cmd, description, cb);
	m_mapSubParser[sub_cmd] = pSub;
	return *pSub;
}

void CArgParser::ParseArgs()
{
	if (ParseArgs(1) == false)
		exit(-1);
}

CSmartType &CArgParser::operator [] (const CString& key)
{
	map<CString, list<SArgInfo>::iterator>::iterator it = m_mapKV.find(key);
	if (it == m_mapKV.end())
		return NONE;
	return it->second->value;
}

void CArgParser::PrintHelp()
{
	cout << "usage: " << m_strName;
	for (list<SArgInfo>::iterator it = m_lsArgInfo.begin(); it != m_lsArgInfo.end(); ++it)
	{
		if (it->required)
			printf(" --%s", (PCC)it->name);
		else
			printf(" [--%s]", (PCC)it->name);
	}
	for (list<SArgInfo>::iterator it = m_lsPositional.begin(); it != m_lsPositional.end(); ++it)
		printf(" <%s>", (PCC)it->name);
	cout << " {";
	for (map<CString, CArgParser*>::iterator it = m_mapSubParser.begin(); it != m_mapSubParser.end(); ++it)
		printf("%s,", (PCC)it->first);
	cout << "}" << endl << endl;
	if (!m_strDescription.Empty())
	{
		cout << "description:" << endl << "\t";
		cout << m_strDescription << endl << endl;
	}

	cout << "positional arguments:" << endl;
	for (list<SArgInfo>::iterator it = m_lsPositional.begin(); it != m_lsPositional.end(); ++it)
	{
		CString str = it->type == ARG_TYPE_INT || it->type == ARG_TYPE_FLOAT ? "digit_" : "string_";
		str += it->is_list ? "list" : "value";
		printf("<%s %s>\t%s\n", (PCC)it->name, (PCC)str, (PCC)it->help);
	}
	cout << "{";
	for (map<CString, CArgParser*>::iterator it = m_mapSubParser.begin(); it != m_mapSubParser.end(); ++it)
		printf("%s,", (PCC)it->first);
	cout << "} commands" << endl << endl;
	cout << "optional arguments:" << endl;
	for (list<SArgInfo>::iterator it = m_lsArgInfo.begin(); it != m_lsArgInfo.end(); ++it)
	{
		printf("--%s\t-%c ", (PCC)it->name, it->short_name);
		if (it->type != ARG_TYPE_BOOL)
		{
			CString str = it->type == ARG_TYPE_INT || it->type == ARG_TYPE_FLOAT ? "digit_" : "string_";
			str += it->is_list ? "list" : "value";
			printf("<%s>", (PCC)str);
		}
		printf(" : %s\n", (PCC)it->help);
	}
	cout << endl;
}

void CArgParser::PrintResult()
{
	cout << m_strName << ":" << endl;
	for (list<SArgInfo>::iterator it = m_lsPositional.begin(); it != m_lsPositional.end(); ++it)
	{
		printf("%s = %s\n", (PCC)it->name, it->value.ToStr());
	}
	cout << endl;
	for (list<SArgInfo>::iterator it = m_lsArgInfo.begin(); it != m_lsArgInfo.end(); ++it)
	{
		printf("%s = %s\n", (PCC)it->name, it->value.ToStr());
	}
	cout << "= = = = = = = = = = = = = = = =" << endl;
	if (m_pSubParser)
	{
		m_pSubParser->PrintResult();
	}
}

bool CArgParser::ParseArgs(int nBeg)
{
	list<const char*> lsPos;
	for (int i = nBeg; i < m_nArg; ++i)
	{
		CString str(m_pArgs[i]);
		if (str.StartsWith("--"))
		{
			int nEq = str.Find('=');
			map<CString, list<SArgInfo>::iterator>::iterator itKey = nEq == -1 ? m_mapKV.find(&str[2]) : m_mapKV.find(str.SubStr(2, nEq));

			if (itKey == m_mapKV.end())
			{
				cout << "unknown option: " << str << endl;
				return false;
			}

			if (itKey->second->type == ARG_TYPE_BOOL)
				itKey->second->value = true;
			else
				ParseValue(*itKey->second, nEq == -1 ? m_pArgs[++i] : &str[nEq + 1]);
		}
		else if (str.StartsWith("-"))
		{
			map<CString, list<SArgInfo>::iterator>::iterator itKey;
			for (int j = 1; j < str.GetLength(); ++j)
			{
				itKey = m_mapKV.find(str[j]);
				if (itKey == m_mapKV.end())
				{
					cout << "unknown option: " << str << endl;
					return false;
				}
				if (itKey->second->type != ARG_TYPE_BOOL)
				{
					ParseValue(*itKey->second, j < str.GetLength()-1 ? &str[j + 1] : m_pArgs[++i]);
					break;
				}
				itKey->second->value = true;
			}
		}
		else
		{
			const char* p = m_pArgs[i];
			map<CString, CArgParser*>::iterator itSub = m_mapSubParser.find(p);
			if (itSub == m_mapSubParser.end())
			{
				lsPos.push_back(p);
			}
			else
			{
				m_pSubParser = itSub->second;
				return m_pSubParser->ParseArgs(++i);
			}
		}
		if ((*this)["help"])
		{
			PrintHelp();
			exit(0);
		}
	}
	for (list<SArgInfo>::iterator it = m_lsArgInfo.begin(); it != m_lsArgInfo.end(); ++it)
	{
		if (it->required && it->value == NONE)
		{
			printf("--%s/-%c is required\n", (PCC)it->name, (PCC)it->short_name);
			exit(-1);
		}
	}
	list<SArgInfo>::iterator itInfo = m_lsPositional.begin();
	list<const char*>::iterator itPos = lsPos.begin();
	int nListPos = (int)lsPos.size() - (int)m_lsPositional.size();
	if (nListPos < 0)
	{
		cout << "no enough positional arguments" << endl;
		return false;
	}
	for (; itInfo != m_lsPositional.end(); ++itInfo, ++itPos)
	{
		if (itInfo->is_list)
			for (int i = 0; i < nListPos; ++i, ++itPos)
				ParseValue(*itInfo, *itPos);

		ParseValue(*itInfo, *itPos);
		m_mapKV[itInfo->name] = itInfo;
	}

	//cout << "Parse Args Finish!" << endl;
	if (m_cb)
		exit(m_cb(*this));
	return true;
}

void CArgParser::ParseValue(SArgInfo &arg_info, const char * str)
{
	if (arg_info.is_list)
	{
		REF_LIST ls = arg_info.value;
		switch (arg_info.type) {
		case ARG_TYPE_INT:
			ls.push_back(atoi(str));
			break;
		case ARG_TYPE_FLOAT:
			ls.push_back(atof(str));
			break;
		default:
			ls.push_back(str);
			break;
		}
	}
	else
	{
		switch (arg_info.type) {
		case ARG_TYPE_INT:
			arg_info.value = atoi(str);
			break;
		case ARG_TYPE_FLOAT:
			arg_info.value = atof(str);
			break;
		default:
			arg_info.value = str;
			break;
		}
	}
}
