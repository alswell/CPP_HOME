#ifndef __ARGS_H
#define __ARGS_H
#include "afx.h"

#include "dict.h"
#include "str.h"
#include "type.h"

class CArgParser
{
	CString m_strName;
    DICT(CStr) m_kk;
	DICT(CValueType) m_kv;
	vector<CString> m_vName;
	int m_iName;
	DICT(CArgParser*) m_dictSubParsers;
	CArgParser* m_pSubParser;
public:
	CArgParser();

	void AddOption(CString op, EValueType type = TYPE_STRING, CString alias = "");
	void AddArg(CString name);
    CArgParser &AddSub(CString sub);
	bool ParseArgs(int argc, char* argv[]);

//	CStr Get(const CStr& key, const char* strDefault = NULL);

//	CStr &operator [] (const CStr& key);
	CArgParser &operator () (const CString& sub);

	void PrintHelp();
	void PrintResult();
private:
	void ParseValue(CValueType& value, char *argv[], int& i);
};


#endif
