#pragma once
#include "env.h"

#include "dict.h"
#include "str.h"
#include "type.h"


class IArg
{
public:
	const char* m_strName;
	char m_cName;
	bool m_bRequired;
	bool m_bSet;
	const char* m_strHelp;
public:
	void SetName(const char* name, char short_name, bool required, const char* help)
	{
		m_strName = name;
		m_cName = short_name;
		m_bRequired = required;
		m_strHelp = help;
	}
	virtual bool IsBool() { return false; }
	virtual ~IArg() {}
	virtual void SetValue(const char* str) = 0;
	virtual const char* Type() = 0;
};

class CArgBool : public IArg
{
public:
	bool m_bDefault;
	bool m_bValue;
public:
	virtual bool IsBool() { return true; }
	virtual void SetValue(const char*)
	{
		m_bValue = !m_bDefault;
	}
	virtual const char* Type()
	{
		return "";
	}
};

class CArgInt : public IArg
{
public:
	int m_iDefault;
	int m_iValue;
	virtual void SetValue(const char* str)
	{
		m_iValue = atoi(str);
	}
	virtual const char* Type()
	{
		return "int value";
	}
};

class CArgFloat : public IArg
{
public:
	double m_fDefault;
	double m_fValue;
	virtual void SetValue(const char* str)
	{
		m_fValue = atof(str);
	}
	virtual const char* Type()
	{
		return "float value";
	}
};

class CArgStr : public IArg
{
public:
	const char* m_strDefault;
	const char* m_strValue;
	virtual void SetValue(const char* str)
	{
		m_strValue = str;
	}
	virtual const char* Type()
	{
		return "string value";
	}
};


class CArgParser
{
	typedef int(*CB)(CArgParser& args);
	CB m_cb;
	static int m_nArg;
	static char** m_pArgs;
	bool* m_bPrintHelp;
	CString m_strName;
	CString m_strDescription;
	list<IArg*> m_lsArgInfo;
	map<CString, IArg*> m_mapKV;
	list<IArg*> m_lsPositional;
	//SArgInfo* m_pListPosition;
	map<CString, CArgParser*> m_mapSubParser;
	CArgParser* m_pSubParser;

	CArgParser(const char* sub_cmd, const char* description, CB cb);
public:
	CArgParser(int argc, char* argv[], const char *description = NULL);

	void AddOption(IArg* p);
	bool* AddBool(const char* name, char short_name = 0, const char* help = nullptr);
	int* AddInt(const char* name, char short_name = 0, bool required = false, const char* help = nullptr);
	double* AddFloat(const char* name, char short_name = 0, bool required = false, const char* help = nullptr);
	const char** AddStr(const char* name, char short_name = 0, bool required = false, const char* help = nullptr);
	//void AddPosition(const char* name, EArgType type = ARG_TYPE_STR, bool is_list = false, const char* help = NULL);
	CArgParser &AddSub(const char *sub_cmd, CB cb, const char *description = NULL);
	void ParseArgs();

//	CSmartType &operator [] (const CString &key);

	void PrintHelp();
	void PrintResult();
private:
	bool ParseArgs(int nBeg);
};


