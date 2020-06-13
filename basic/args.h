#pragma once
#include "env.h"


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

class CArgIntList : public IArg
{
public:
	virtual void SetValue(const char* str);
	virtual const char* Type();
};

class CArgFloatList : public IArg
{
public:
	virtual void SetValue(const char* str);
	virtual const char* Type();
};

class CArgStrList : public IArg
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
	size_t m_nMaxFlagLen;
	const char* m_strName;
	const char* m_strDescription;
	list<IArg*> m_lsArgInfo;
	map<const char*, IArg*, StrCMP> m_mapKV;
	map<char, IArg*> m_mapKVc;
	list<IArg*> m_lsPositional;
	IArg* m_pListPosition;
	map<const char*, CArgParser*, StrCMP> m_mapSubParser;
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
	IArg* Add(list<int>& value, const char* name, char short_name = 0, bool required = false, const char* help = nullptr);
	IArg* Add(list<double>& value, const char* name, char short_name = 0, bool required = false, const char* help = nullptr);
	IArg* Add(list<const char*>& value, const char* name, char short_name = 0, bool required = false, const char* help = nullptr);
	IArg* AddPosition(int& value, const char* name, const char* help = nullptr);
	IArg* AddPosition(double& value, const char* name, const char* help = nullptr);
	IArg* AddPosition(const char*& value, const char* name, const char* help = nullptr);
	IArg* AddPosition(list<int>& value, const char* name, const char* help = nullptr);
	IArg* AddPosition(list<double>& value, const char* name, const char* help = nullptr);
	IArg* AddPosition(list<const char*>& value, const char* name, const char* help = nullptr);
	CArgParser& AddSub(const char *sub_cmd, CB cb, const char *description = nullptr);
	void ParseArgs();

	void PrintHelp();
private:
	bool ParseArgs(int nBeg);
};
