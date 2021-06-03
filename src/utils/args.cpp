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

void CArgBool::PrintValue()
{
	cout << (*reinterpret_cast<bool*>(m_pValue) ? "true" : "false");
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

void CArgInt::PrintValue()
{
	cout << *reinterpret_cast<int*>(m_pValue);
}

const char* CArgInt::Type()
{
	return "(int value)";
}

void CArgFloat::SetValue(const char* str)
{
	auto p = reinterpret_cast<double*>(m_pValue);
	*p = atof(str);
}

void CArgFloat::PrintValue()
{
	cout << *reinterpret_cast<double*>(m_pValue);
}

const char* CArgFloat::Type()
{
	return "(float value)";
}

void CArgStr::SetValue(const char* str)
{
	auto p = reinterpret_cast<const char**>(m_pValue);
	*p = str;
}

void CArgStr::PrintValue()
{
	cout << *reinterpret_cast<const char**>(m_pValue);
}

const char* CArgStr::Type()
{
	return "(string value)";
}

void CArgIntList::SetValue(const char* str)
{
	auto p = reinterpret_cast<list<int>*>(m_pValue);
	p->push_back(atoi(str));
}

void CArgIntList::PrintValue()
{
	auto p = reinterpret_cast<list<int>*>(m_pValue);
	cout << '[';
	for (auto it = p->begin(); it != p->end(); ++it)
	{
		if (it != p->begin())
			cout << ' ';
		cout << *it;
	}
	cout << ']';
}

const char* CArgIntList::Type()
{
	return "(int list)";
}

void CArgFloatList::SetValue(const char* str)
{
	auto p = reinterpret_cast<list<double>*>(m_pValue);
	p->push_back(atof(str));
}

void CArgFloatList::PrintValue()
{
	auto p = reinterpret_cast<list<double>*>(m_pValue);
	cout << '[';
	for (auto it = p->begin(); it != p->end(); ++it)
	{
		if (it != p->begin())
			cout << ' ';
		cout << *it;
	}
	cout << ']';
}

const char* CArgFloatList::Type()
{
	return "(float list)";
}

void CArgStrList::SetValue(const char* str)
{
	auto p = reinterpret_cast<list<const char*>*>(m_pValue);
	p->push_back(str);
}

void CArgStrList::PrintValue()
{
	auto p = reinterpret_cast<list<const char*>*>(m_pValue);
	cout << '[';
	for (auto it = p->begin(); it != p->end(); ++it)
	{
		if (it != p->begin())
			cout << ' ';
		cout << *it;
	}
	cout << ']';
}

const char* CArgStrList::Type()
{
	return "(string list)";
}

int CArgParser::m_nArg;
char** CArgParser::m_pArgs;
CArgParser::CArgParser(const char *sub_cmd, const char *description, CB cb)
	: m_bPrintHelp(false)
{
	Add(m_bPrintHelp, "help", 'h', "show help message");
	m_nMaxFlagLen = 4;

	m_strName = sub_cmd;
	m_strDescription = description;

	m_pListPosition = nullptr;
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
	if (p->m_cName)
	{
		if ((p->m_cName < 'a' || p->m_cName > 'z') && (p->m_cName < 'A' || p->m_cName > 'Z'))
		{
			cout << "CArgParser:: invalid short name, only support [a-z] and [A-Z]" << endl;
			exit(-1);
		}
		auto it = m_mapKVc.find(p->m_cName);
		if (it != m_mapKVc.end())
		{
			cout << "CArgParser:: duplicate key: " << p->m_cName << endl;
			exit(-1);
		}
		m_mapKVc[p->m_cName] = p;
	}
	m_lsArgInfo.push_back(p);
	m_mapKV[p->m_strName] = p;
	if (m_nMaxFlagLen < strlen(p->m_strName))
		m_nMaxFlagLen = strlen(p->m_strName);
}

#define ADD_FLAG(cls) \
auto p = new cls;\
p->Init(name, short_name, required, help, &value);\
AddOption(p);\
return p

#define ADD_POSITION(cls) \
auto p = new cls;\
p->Init(name, 0, true, help, &value);\
m_lsPositional.push_back(p);\
return p

#define ADD_POSITION_LIST(cls) \
if (m_pListPosition)\
{\
	cout << "Only one positional argument can be defined as list!" << endl;\
	exit(-1);\
}\
auto p = new cls;\
p->Init(name, 0, true, help, &value);\
m_lsPositional.push_back(p);\
m_pListPosition = p;\
return p


IArg* CArgParser::Add(bool& value, const char* name, char short_name, const char* help)
{
	bool required = false;
	ADD_FLAG(CArgBool);
}

IArg* CArgParser::Add(int& value, const char* name, char short_name, bool required, const char* help)
{
	ADD_FLAG(CArgInt);
}

IArg* CArgParser::Add(double& value, const char* name, char short_name, bool required, const char* help)
{
	ADD_FLAG(CArgFloat);
}

IArg* CArgParser::Add(const char*& value, const char* name, char short_name, bool required, const char* help)
{
	ADD_FLAG(CArgStr);
}

IArg* CArgParser::Add(list<int>& value, const char* name, char short_name, bool required, const char* help)
{
	ADD_FLAG(CArgIntList);
}

IArg* CArgParser::Add(list<double>& value, const char* name, char short_name, bool required, const char* help)
{
	ADD_FLAG(CArgFloatList);
}

IArg* CArgParser::Add(list<const char*>& value, const char* name, char short_name, bool required, const char* help)
{
	ADD_FLAG(CArgStrList);
}

IArg* CArgParser::AddPosition(int& value, const char* name, const char* help)
{
	ADD_POSITION(CArgInt);
}

IArg* CArgParser::AddPosition(double& value, const char* name, const char* help)
{
	ADD_POSITION(CArgFloat);
}

IArg* CArgParser::AddPosition(const char*& value, const char* name, const char* help)
{
	ADD_POSITION(CArgStr);
}

IArg* CArgParser::AddPosition(list<int>& value, const char* name, const char* help)
{
	ADD_POSITION_LIST(CArgIntList);
}

IArg* CArgParser::AddPosition(list<double>& value, const char* name, const char* help)
{
	ADD_POSITION_LIST(CArgFloatList);
}

IArg* CArgParser::AddPosition(list<const char*>& value, const char* name, const char* help)
{
	ADD_POSITION_LIST(CArgStrList);
}

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

char short_flag[] = "/- ";
const char* print_short(char c)
{
	if (c == 0) return "";
	short_flag[2] = c;
	return short_flag;
}
void CArgParser::PrintHelp()
{
	cout << "usage: ";
	for (auto it = m_lsRefParentFlag.begin(); it != m_lsRefParentFlag.end(); ++it)
	{
		printf("--%s ", (**it).m_strName);
	}
	cout << m_strName;
	for (auto it = m_lsArgInfo.begin(); it != m_lsArgInfo.end(); ++it)
	{
		if ((**it).m_bRequired)
			printf(" --%s", (**it).m_strName);
		else
			printf(" [--%s]", (**it).m_strName);
	}
	for (auto it = m_lsPositional.begin(); it != m_lsPositional.end(); ++it)
		printf(" <%s>", (**it).m_strName);
	cout << " {";
	for (auto it = m_mapSubParser.begin(); it != m_mapSubParser.end(); ++it)
		printf("%s,", it->first);
	cout << "}" << endl;

	if (m_strDescription)
	{
		cout << endl << "description:" << endl;
		cout << "  " << m_strDescription << endl;
	}

	if (!m_lsPositional.empty())
	{
		cout << endl << "positional arguments:" << endl;
		for (auto it = m_lsPositional.begin(); it != m_lsPositional.end(); ++it)
			printf("%16s %s: %s\n", (**it).m_strName, (**it).Type(), (**it).m_strHelp);
	}

	if (!m_mapSubParser.empty())
	{
		cout << endl << "subcommands:" << endl;
		for (auto it = m_mapSubParser.begin(); it != m_mapSubParser.end(); ++it)
			printf(" %16s: %s\n", it->second->m_strName, it->second->m_strDescription);
	}

	if (!m_lsRefParentFlag.empty())
	{
		cout << endl << "depend on:" << endl;
		for (auto it = m_lsRefParentFlag.begin(); it != m_lsRefParentFlag.end(); ++it)
			cout << "--" << (**it).m_strName << print_short((**it).m_cName) << endl;
	}

	cout << endl << "optional arguments:" << endl;
	char strFmtFlag[32];
	sprintf(strFmtFlag, "--%%-%zus ", m_nMaxFlagLen);
	for (auto it = m_lsArgInfo.begin(); it != m_lsArgInfo.end(); ++it)
	{
		printf(strFmtFlag, (**it).m_strName);
		if ((**it).m_cName)
			printf("-%c ", (**it).m_cName);
		else
			cout << "   ";
		printf("%14s: %s", (**it).Type(), (**it).m_strHelp);
		if ((**it).m_bRequired)
			cout << " (mandatory)" << endl;
		else
			cout << " (default: ", (**it).PrintValue(), cout << ")" << endl;
	}
	cout << endl;
}

bool CArgParser::ParseArgs(int nBeg)
{
	list<const char*> lsPos;
	for (int i = nBeg; i < m_nArg; ++i)
	{
		if (m_pArgs[i][0] == '-')
		{
			if (m_pArgs[i][1] == '-')
			{
				auto p = strchr(m_pArgs[i], '=');
				if (p) *p = 0;
				auto itKey = m_mapKV.find(&m_pArgs[i][2]);

				if (itKey == m_mapKV.end())
				{
					cout << "unknown option: " << &m_pArgs[i][2] << endl;
					return false;
				}

				itKey->second->m_bSet = true;
				if (itKey->second->IsBool())
					itKey->second->SetValue(nullptr);
				else
					itKey->second->SetValue(p ? p+1 : m_pArgs[++i]);
			}
			else
			{
				auto len = strlen(m_pArgs[i]);
				for (size_t j = 1; j < len; ++j)
				{
					auto itKey = m_mapKVc.find(m_pArgs[i][j]);
					if (itKey == m_mapKVc.end())
					{
						cout << "unknown option: " << m_pArgs[i][j] << endl;
						return false;
					}
					itKey->second->m_bSet = true;
					if (!itKey->second->IsBool())
					{
						itKey->second->SetValue(j < len - 1 ? &(m_pArgs[i])[j + 1] : m_pArgs[++i]);
						break;
					}
					itKey->second->SetValue(nullptr);
				}
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
				auto pSubParser = itSub->second;
				return pSubParser->ParseArgs(++i);
			}
		}
		if (m_bPrintHelp)
		{
			m_bPrintHelp = false;
			PrintHelp();
			exit(0);
		}
	}
	bool quit = false;
	for (auto it = m_lsRefParentFlag.begin(); it != m_lsRefParentFlag.end(); ++it)
	{
		if (!(**it).m_bSet)
		{
			quit = true;
			printf("subcommand '%s' depends on --%s%s\n", m_strName, (**it).m_strName, print_short((**it).m_cName));
		}
	}
	for (auto it = m_lsArgInfo.begin(); it != m_lsArgInfo.end(); ++it)
	{
		if ((**it).m_bRequired && !(**it).m_bSet)
		{
			quit = true;
			printf("--%s%s is required\n", (**it).m_strName, print_short((**it).m_cName));
		}
	}
	if (quit)
	{
		cout << "try --help/-h for more informatiion" << endl;
		exit(-1);
	}
	int nListPos = int(lsPos.size()) - int(m_lsPositional.size());
	if (nListPos < 0)
	{
		cout << "no enough positional arguments" << endl;
		cout << "try --help/-h for more informatiion" << endl;
		return false;
	}
	auto itInfo = m_lsPositional.begin();
	auto itPos = lsPos.begin();
	for (; itInfo != m_lsPositional.end(); ++itInfo, ++itPos)
	{
		if (m_pListPosition == *itInfo)
			for (int i = 0; i < nListPos; ++i, ++itPos)
				(**itInfo).SetValue(*itPos);

		(**itInfo).SetValue(*itPos);
	}

	//cout << "Parse Args Finish!" << endl;
	if (m_cb)
		exit(m_cb());
	return true;
}
