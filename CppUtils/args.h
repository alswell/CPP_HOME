#ifndef __ARGS_H
#define __ARGS_H
#include "afx.h"

#include "dict.h"
#include "str.h"
#include "type.h"

class CArgParser
{
	CStr m_strName;
    DICT(CStr) m_kk;
	DICT(CValueType) m_kv;
	vector<CStr> m_vName;
	int m_iName;
	DICT(CArgParser*) m_dictSubParsers;
	CArgParser* m_pSubParser;
public:
	CArgParser();

	void AddOption(CStr op, EValueType type = TYPE_STRING, CStr alias = "");
	void AddArg(CStr name);
    CArgParser &AddSub(CStr sub);
	bool ParseArgs(int argc, char* argv[]);

//	CStr Get(const CStr& key, const char* strDefault = NULL);

//	CStr &operator [] (const CStr& key);
	CArgParser &operator () (const CStr& sub);

	void PrintHelp();
	void PrintResult();
private:
	void ParseValue(CValueType& value, char *argv[], int& i);
};


#endif
