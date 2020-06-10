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
	void* m_pValue;
public:
	virtual ~IArg();
	void Init(const char* name, char short_name, bool required, const char* help, void* pValue);
	virtual bool IsBool();
	virtual void SetValue(const char* str) = 0;
	virtual const char* Type() = 0;
};

class CArgBool : public IArg
{
public:
	virtual bool IsBool();
	virtual void SetValue(const char*);
	virtual const char* Type();
};

class CArgInt : public IArg
{
public:
	virtual void SetValue(const char* str);
	virtual const char* Type();
};

class CArgFloat : public IArg
{
public:
	virtual void SetValue(const char* str);
	virtual const char* Type();
};

class CArgStr : public IArg
{
public:
	virtual void SetValue(const char* str);
	virtual const char* Type();
};


class CArgParser
{
	typedef int(*CB)(void);
	CB m_cb;
	static int m_nArg;
	static char** m_pArgs;
	bool m_bPrintHelp;
	int m_nMaxFlagLen;
	CString m_strName;
	CString m_strDescription;
	list<IArg*> m_lsArgInfo;
	map<CString, IArg*> m_mapKV;
	list<IArg*> m_lsPositional;
	//SArgInfo* m_pListPosition;
	map<CString, CArgParser*> m_mapSubParser;
	CArgParser* m_pSubParser;
	list<IArg*> m_lsRefParentFlag;

	CArgParser(const char* sub_cmd, const char* description, CB cb);
public:
	CArgParser(int argc, char* argv[], const char *description = nullptr);

	CArgParser& AddRef(IArg* p);
	void AddOption(IArg* p);
	IArg* Add(bool& value, const char* name, char short_name = 0, const char* help = nullptr);
	IArg* Add(int& value, const char* name, char short_name = 0, bool required = false, const char* help = nullptr);
	IArg* Add(double& value, const char* name, char short_name = 0, bool required = false, const char* help = nullptr);
	IArg* Add(const char*& value, const char* name, char short_name = 0, bool required = false, const char* help = nullptr);
	//void AddPosition(const char* name, EArgType type = ARG_TYPE_STR, bool is_list = false, const char* help = NULL);
	CArgParser& AddSub(const char *sub_cmd, CB cb, const char *description = nullptr);
	void ParseArgs();

	void PrintHelp();
private:
	bool ParseArgs(int nBeg);
};

