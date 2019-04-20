#include "conf.h"
#include "file.h"

CConf* CConf::m_pStatic = 0;
CString CConf::m_strDefaultFile;
CConf::CConf(const char* filename)
	: m_strFileName(filename)
{
	CFile file(filename);
	list<CString> ls = file.ReadLines();
	FOR_LIST(CString, ls, it)
	{
		(*it).Trim();
		if ((*it).GetLength() == 0)
			continue;
		if ((*it)[0] == '[' && (*it)[-1] == ']')
		{
			m_lsSection.push_back((*it).SubStr(1, -1));
		}
		else if ((*it)[0] != '#')
		{
			list<CString> lsKV =(*it).Split('=');
			if (lsKV.size() == 2)
				m_kv[m_lsSection.back()][lsKV.front().Trim()] = lsKV.back().Trim();
		}
	}
	file.Close();

//	FOR_LIST(CString, ls, it)
//	{
//		it->Trim();
//		if (it->GetLength() == 0)
//			continue;
//		if ((*it)[0] == '[' && (*it)[-1] == ']')
//		{
//			strSection = it->SubStr(1, -1);
//		}
//		else if ((*it)[0] != '#')
//		{
//			arr<CString> lsKV = it->Split('=');
//			if (lsKV.size() == 2)
//				m_kv[strSection][lsKV[0].Trim()] = lsKV[1].Trim();
//		}
//	}
}

CConf::~CConf()
{
}

void CConf::SetDefault(const char *strFileName)
{
	m_strDefaultFile = strFileName;
}

CConf& CConf::Instance()
{
	if (!m_pStatic && !m_strDefaultFile.Empty())
		m_pStatic = new CConf(m_strDefaultFile);
	return *m_pStatic;
}

DICT(CString) &CConf::operator [](CString section)
{
	return m_kv[section];
}

void AppendHelper(list<CString>& ls, list<CString>::iterator it, DICT(CString)& dict)
{
	CString str;
	FOR_DICT(CString, dict, it_dict)
	{
		str.AppendFormat("\n%s = %s", (const char*)it_dict->first, (const char*)it_dict->second);
	}
	if (str.Empty() || it == ls.begin())
		return;
	for (--it; it != ls.begin(); --it)
	{
		if (!(*it).Empty())
		{
			(*it) += str;
			break;
		}
	}
}
void CConf::Save()
{
	CFile file1(m_strFileName);
	list<CString> ls = file1.ReadLines();
	file1.Close();
	CString strSection;
	DICT(DICT(CString)) dict = m_kv;
	FOR_LIST(CString, ls, it)
	{
		(*it).Trim();
		if ((*it).GetLength() == 0)
			continue;
		if ((*it)[0] == '[' && (*it)[-1] == ']')
		{
			AppendHelper(ls, it, dict[strSection]);
			dict.erase(strSection);
			strSection = (*it).SubStr(1, -1);
		}
		else if ((*it)[0] != '#')
		{
			list<CString> lsKV =(*it).Split('=');
			if (lsKV.size() == 2)
			{
				lsKV.front().Trim();
				CString str;
				str.Format("%s = %s", (const char*)lsKV.front(), (const char*)dict[strSection][lsKV.front()]);
				dict[strSection].erase(lsKV.front());
				(*it) = str;
			}
		}
	}
	AppendHelper(ls, ls.end(), dict[strSection]);
	dict.erase(strSection);

	CFile file(m_strFileName, "wb");

//	FOR_LIST(CString, m_lsSection, it)
//	{
//		CString strSection;
//		strSection.Format("[%s]", (const char*)*it);
//		file.WriteLine(strSection);
//		DICT(CString)& dict = m_kv[*it];
//		FOR_DICT(CString, dict, it_dict)
//		{
//			CString str;
//			str.Format("%s=%s", (const char*)it_dict->first, (const char*)it_dict->second);
//			file.WriteLine(str);
//		}
//		file.WriteLine("");
//	}
	FOR_LIST(CString, ls, it)
	{
		file.WriteLine((*it));
	}
	for (DICT(DICT(CString))::iterator it = dict.begin(); it != dict.end(); ++it)
	{
		CString strSection;
		strSection.Format("[%s]", (const char*)it->first);
		file.WriteLine(strSection);
		FOR_DICT(CString, it->second, it_dict)
		{
			CString str;
			str.Format("%s = %s", (const char*)it_dict->first, (const char*)it_dict->second);
			file.WriteLine(str);
		}
		file.WriteLine("");
	}
	file.Close();
}
