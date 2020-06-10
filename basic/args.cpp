#include "args.h"

IArg::~IArg()
{
}

void IArg::Init(const char* name, char short_name, bool required, const char* help, void* pValue)
{
	m_strName = name;
	m_cName = short_name;
	m_bRequired = required;
	m_strHelp = help;
	m_pValue = pValue;
}

bool IArg::IsBool()
{
	return false;
}

bool CArgBool::IsBool()
{
	return true;
}

void CArgBool::SetValue(const char*)
{
	auto p = reinterpret_cast<bool*>(m_pValue);
	*p = !*p;
}

const char* CArgBool::Type()
{
	return "";
}

void CArgInt::SetValue(const char* str)
{
	auto p = reinterpret_cast<int*>(m_pValue);
	*p = atoi(str);
}

const char* CArgInt::Type()
{
	return "<int value>";
}

void CArgFloat::SetValue(const char* str)
{
	auto p = reinterpret_cast<double*>(m_pValue);
	*p = atof(str);
}

const char* CArgFloat::Type()
{
	return "<float value>";
}

void CArgStr::SetValue(const char* str)
{
	auto p = reinterpret_cast<const char**>(m_pValue);
	*p = str;
}

const char* CArgStr::Type()
{
	return "<string value>";
}

#define PCC const char*
int CArgParser::m_nArg;
char** CArgParser::m_pArgs;
CArgParser::CArgParser(const char *sub_cmd, const char *description, CB cb)
{
	Add(m_bPrintHelp, "help", 'h', "show help message");
	m_nMaxFlagLen = 4;

	m_strName = sub_cmd;
	m_strDescription = description;

	m_pSubParser = nullptr;
	m_cb = cb;
}

CArgParser::CArgParser(int argc, char *argv[], const char *description)
	: CArgParser(argv[0], description, nullptr)
{
	m_nArg = argc;
	m_pArgs = argv;
}

CArgParser& CArgParser::AddRef(IArg* p)
{
	m_lsRefParentFlag.push_back(p);
	return *this;
}

void CArgParser::AddOption(IArg* p)
{
	auto it = m_mapKV.find(p->m_strName);
	if (it != m_mapKV.end())
	{
		cout << "CArgParser:: duplicate key: " << p->m_strName << endl;
		exit(-1);
	}
	it = m_mapKV.find(p->m_cName);
	if (it != m_mapKV.end())
	{
		cout << "CArgParser:: duplicate key: " << p->m_cName << endl;
		exit(-1);
	}
	m_lsArgInfo.push_back(p);
	m_mapKV[p->m_strName] = p;
	m_mapKV[p->m_cName] = p;
	if (m_nMaxFlagLen < strlen(p->m_strName))
		m_nMaxFlagLen = strlen(p->m_strName);
}

IArg* CArgParser::Add(bool& value, const char* name, char short_name, const char* help)
{
	auto p = new CArgBool;
	p->Init(name, short_name, false, help, &value);
	AddOption(p);
	return p;
}

IArg* CArgParser::Add(int& value, const char* name, char short_name, bool required, const char* help)
{
	auto p = new CArgInt;
	p->Init(name, short_name, required, help, &value);
	AddOption(p);
	return p;
}

IArg* CArgParser::Add(double& value, const char* name, char short_name, bool required, const char* help)
{
	auto p = new CArgFloat;
	p->Init(name, short_name, required, help, &value);
	AddOption(p);
	return p;
}

IArg* CArgParser::Add(const char*& value, const char* name, char short_name, bool required, const char* help)
{
	auto p = new CArgStr;
	p->Init(name, short_name, required, help, &value);
	AddOption(p);
	return p;
}

//void CArgParser::AddPosition(const char *name, EArgType type, bool is_list, const char *help)
//{
//	if (m_pListPosition && is_list)
//	{
//		cout << "Only one positional argument can be defined as list!" << endl;
//		exit(-1);
//	}
//	SArgInfo info = {name, 0, type, true, is_list, help, GetInitVal(type, is_list)};
//	m_lsPositional.push_back(info);
//	if (is_list)
//		m_pListPosition = &m_lsPositional.back();
//}

CArgParser& CArgParser::AddSub(const char* sub_cmd, CB cb, const char* description)
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

void CArgParser::PrintHelp()
{
	cout << "usage: " << m_strName;
	for (auto it = m_lsArgInfo.begin(); it != m_lsArgInfo.end(); ++it)
	{
		if ((**it).m_bRequired)
			printf(" --%s", (PCC)(**it).m_strName);
		else
			printf(" [--%s]", (PCC)(**it).m_strName);
	}
	for (auto it = m_lsPositional.begin(); it != m_lsPositional.end(); ++it)
		printf(" <%s>", (PCC)(**it).m_strName);
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
	for (auto it = m_lsPositional.begin(); it != m_lsPositional.end(); ++it)
	{
//		CString str = it->type == ARG_TYPE_INT || it->type == ARG_TYPE_FLOAT ? "digit_" : "string_";
//		str += it->is_list ? "list" : "value";
//		printf("<%s %s>\t%s\n", (PCC)it->name, (PCC)str, (PCC)it->help);
	}
	cout << "{";
	for (map<CString, CArgParser*>::iterator it = m_mapSubParser.begin(); it != m_mapSubParser.end(); ++it)
		printf("%s,", (PCC)it->first);
	cout << "} commands" << endl << endl;
	cout << "optional arguments:" << endl;
	CString strFmtFlag;
	strFmtFlag.Format("--%%-%ds -%%c ", m_nMaxFlagLen);
	for (auto it = m_lsArgInfo.begin(); it != m_lsArgInfo.end(); ++it)
	{
		printf(strFmtFlag, (PCC)(**it).m_strName, (**it).m_cName);
		printf("%14s : %s\n", (**it).Type(), (PCC)(**it).m_strHelp);
	}
	cout << endl;
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
			auto itKey = nEq == -1 ? m_mapKV.find(&str[2]) : m_mapKV.find(str.SubStr(2, nEq));

			if (itKey == m_mapKV.end())
			{
				cout << "unknown option: " << str << endl;
				return false;
			}

			itKey->second->m_bSet = true;
			if (itKey->second->IsBool())
				itKey->second->SetValue(nullptr);
			else
				itKey->second->SetValue(nEq == -1 ? m_pArgs[++i] : &(m_pArgs[i])[nEq + 1]);
		}
		else if (str.StartsWith("-"))
		{
			for (int j = 1; j < str.GetLength(); ++j)
			{
				auto itKey = m_mapKV.find(str[j]);
				if (itKey == m_mapKV.end())
				{
					cout << "unknown option: " << str << endl;
					return false;
				}
				itKey->second->m_bSet = true;
				if (!itKey->second->IsBool())
				{
					itKey->second->SetValue(j < str.GetLength()-1 ? &(m_pArgs[i])[j + 1] : m_pArgs[++i]);
					break;
				}
				itKey->second->SetValue(nullptr);
			}
		}
		else
		{
			const char* p = m_pArgs[i];
			auto itSub = m_mapSubParser.find(p);
			if (itSub == m_mapSubParser.end())
			{
				lsPos.push_back(p);
			}
			else
			{
				m_pSubParser = itSub->second;
				auto& flags = m_pSubParser->m_lsRefParentFlag;
				for (auto it = flags.begin(); it != flags.end(); ++it)
				{
					if (!(**it).m_bSet)
					{
						printf("--%s/-%c is required\n", (**it).m_strName, (**it).m_cName);
						exit(-1);
					}
				}
				return m_pSubParser->ParseArgs(++i);
			}
		}
		if (m_bPrintHelp)
		{
			PrintHelp();
			exit(0);
		}
	}
	for (auto it = m_lsArgInfo.begin(); it != m_lsArgInfo.end(); ++it)
	{
		if ((**it).m_bRequired && !(**it).m_bSet)
		{
			printf("--%s/-%c is required\n", (PCC)(**it).m_strName, (**it).m_cName);
			exit(-1);
		}
	}
	auto itInfo = m_lsPositional.begin();
	list<const char*>::iterator itPos = lsPos.begin();
	int nListPos = (int)lsPos.size() - (int)m_lsPositional.size();
	if (nListPos < 0)
	{
		cout << "no enough positional arguments" << endl;
		return false;
	}
	for (; itInfo != m_lsPositional.end(); ++itInfo, ++itPos)
	{
//		if (itInfo->is_list)
//			for (int i = 0; i < nListPos; ++i, ++itPos)
//				ParseValue(*itInfo, *itPos);

		(**itInfo).SetValue(*itPos);
	}

	//cout << "Parse Args Finish!" << endl;
	if (m_cb)
		exit(m_cb());
	return true;
}
