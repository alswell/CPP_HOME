#include "args.h"


//CArgParser::CArgParser()
//{
//	AddOption("help", TYPE_BOOL, "h");
//	m_iName = 0;
//	m_pSubParser = NULL;
//}

//void CArgParser::AddOption(CString op, EValueType type, CString alias)
//{
//    if (alias == "")
//        alias = op;
//    m_kk[op] = alias;
//	m_kv[alias].m_type = type;
//	m_kv[alias].m_value = 0;
//}

//void CArgParser::AddArg(CString name)
//{
//	m_vName.push_back(name);
//	m_kv[name].m_type = TYPE_STRING;
//	m_kv[name].m_value = 0;
//}

//CArgParser &CArgParser::AddSub(CString sub)
//{
//	m_dictSubParsers[sub] = new CArgParser();
//	return *(m_dictSubParsers[sub]);
//}

//bool CArgParser::ParseArgs(int argc, char *argv[])
//{
//	m_strName = argv[0];
//	for (int i = 1; i < argc; ++i)
//    {
//		CString str(argv[i]);
//        if (str.StartWith("--"))
//        {
//			str = str.Right(2);
//			if (m_kk.find(str) == m_kk.end())
//			{
//				cout << "unknown option: " << str << endl;
//				return false;
//			}
//			CValueType& value = m_kv[m_kk[str]];
//			ParseValue(value, argv, i);
//		}
//        else if (str.StartWith("-"))
//        {
//			str = str.Right(1);
//			if (m_kv.find(str) != m_kv.end())
//			{
//				CValueType& value = m_kv[str];
//				ParseValue(value, argv, i);
//			}
//			else
//			{
//				for (int j = 0; j < str.GetLength() - 1; ++j)
//				{
//					CString alias = str[j];
//					if (m_kv.find(alias) == m_kv.end())
//					{
//						cout << "unknown alias: " << alias << endl;
//						return false;
//					}
//					CValueType& value = m_kv[alias];
//					if (value.m_type != TYPE_BOOL)
//					{
//						cout << "alias should be BOOL: " << alias << endl;
//						return false;
//					}
//					value.m_value = (void*)1;
//				}
//				CString alias = str[-1];
//				if (m_kv.find(alias) == m_kv.end())
//				{
//					cout << "unknown alias: " << alias << endl;
//					return false;
//				}
//				CValueType& value = m_kv[alias];
//				ParseValue(value, argv, i);
//			}
//		}
//        else
//        {
//			if (m_iName < m_vName.size())
//			{
//				CValueType& value = m_kv[m_vName[m_iName]];
//				++m_iName;
//				int j = i - 1;
//				ParseValue(value, argv, j);
//			}
//			else
//			{
//				if (m_dictSubParsers.find(str) == m_dictSubParsers.end())
//				{
//					cout << "unknown sub command: " << str << endl;
//					return false;
//				}
//				m_pSubParser = m_dictSubParsers[str];
//				m_pSubParser->ParseArgs(argc - i, &argv[i]);
//				break;
//			}
//		}
//		if (m_kv['h'].m_value)
//		{
//			PrintHelp();
//			return false;
//		}
//	}
//	return true;
//}

////CString ArgsParser::Get(const CString &key, const char *strDefault)
////{
////	CString& strValue = (*this)[key];
////	if (strValue != "")
////		return strValue;
////	if (strDefault)
////		return CString(strDefault);
////	return "";
////}

////CString &ArgsParser::operator [](const CString &key)
////{
////	return m_kv[key];
////}

//CArgParser &CArgParser::operator ()(const CString &sub)
//{
//	return *m_dictSubParsers[sub];
//}

//void CArgParser::PrintHelp()
//{
//	FOR_DICT(CString, m_kk, it)
//	{
//		if (it->first == "help")
//			continue;
//		CString& alias = m_kk[it->first];
//		CValueType& value = m_kv[alias];
//		cout << "[--" << it->first << "|-" << alias;
//		switch (value.m_type)
//		{
//		case TYPE_BOOL:
//			break;
//		case TYPE_INT:
//			cout << " INT";
//			break;
//		case TYPE_STRING:
//			cout << " STRING";
//			break;
//		default:
//			cout << " UNKNOWN";
//			break;
//		}
//		cout << "] ";
//	}
//	for (int i = 0; i < m_vName.size(); ++i)
//	{
//		cout << m_vName[i] << " ";
//	}
//	cout << "{";
//	FOR_DICT(CArgParser*, m_dictSubParsers, it)
//	{
//		cout << it->first << ", ";
//	}
//	cout << "}" << endl;
//}

//void CArgParser::PrintResult()
//{
//	cout << m_strName << endl;
//	FOR_DICT(CString, m_kk, it)
//	{
//		if (it->first == "help")
//			continue;
//		CString& alias = m_kk[it->first];
//		CValueType& value = m_kv[alias];
//		cout << "--" << it->first << ";\t-" << alias << ":\t";
//		switch (value.m_type)
//		{
//		case TYPE_STRING:
//			cout << value.GetString();
//			break;
//		case TYPE_INT:
//			cout << value.GetInt();
//			break;
//		case TYPE_BOOL:
//			cout << (value.GetBool() ? "True" : "False");
//			break;
//		}
//		cout << endl;
//	}
//	for (int i = 0; i < m_vName.size(); ++i)
//	{
//		CValueType& value = m_kv[m_vName[i]];
//		cout << m_vName[i] << ": " << value.GetString() << endl;
//	}
//	if (m_pSubParser)
//	{
//		m_pSubParser->PrintResult();
//	}
//}

//void CArgParser::ParseValue(CValueType &value, char *argv[], int &i)
//{
//	if (value.m_type == TYPE_BOOL)
//	{
//		value.m_value = (void*)1;
//	}
//	else
//	{
//		++i;
//		switch (value.m_type)
//		{
//		case TYPE_INT:
//			value.m_value = (void*)atol(argv[i]);
//			break;
//		case TYPE_STRING:
//			value.SetString(argv[i]);
//			break;
//		}
//	}
//}
