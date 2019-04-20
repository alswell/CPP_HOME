#pragma once
#include "env.h"

#include "dict.h"
#include "str.h"
#include "type.h"

enum EArgType
{
	ARG_TYPE_BOOL = 0,
	ARG_TYPE_INT,
	ARG_TYPE_FLOAT,
	ARG_TYPE_STR,
};

struct SArgInfo
{
	CString name;
	char short_name;
	char type;
	bool required;
	bool is_list;
	CString help;
	CSmartType value;
};

class CArgParser
{
	typedef int(*CB)(CArgParser& args);
	CB m_cb;
	static int m_nArg;
	static char** m_pArgs;
	CString m_strName;
	CString m_strDescription;
	list<SArgInfo> m_lsArgInfo;
	map<CString, list<SArgInfo>::iterator> m_mapKV;
	list<SArgInfo> m_lsPositional;
	SArgInfo* m_pListPosition;
	map<CString, CArgParser*> m_mapSubParser;
	CArgParser* m_pSubParser;

	CArgParser(const char* sub_cmd, const char* description, CB cb);
public:
	CArgParser(int argc, char* argv[], const char *description = NULL);

	void AddOption(const char* name, char short_name = 0, EArgType type = ARG_TYPE_STR, bool required = false, bool is_list = false, const char* help = NULL);
	void AddPosition(const char* name, EArgType type = ARG_TYPE_STR, bool is_list = false, const char* help = NULL);
	CArgParser &AddSub(const char *sub_cmd, CB cb, const char *description = NULL);
	void ParseArgs();

	CSmartType &operator [] (const CString &key);

	void PrintHelp();
	void PrintResult();
private:
	bool ParseArgs(int nBeg);
	void ParseValue(SArgInfo &arg_info, const char *str);
};


