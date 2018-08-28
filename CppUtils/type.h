#pragma once

#include "env.h"
//#include "dict.h"
#include "str.h"

class CSmartType
{
	class ISmartType
	{
	public:
		virtual ~ISmartType();
		virtual ISmartType* Copy() = 0;
		virtual int ToStr(char* str, int n) = 0;
	};
	ISmartType* m_pSmartTypeImpl;

	class CBOOL : public ISmartType
	{
		friend class CSmartType;
		bool m_value;
	public:
		CBOOL(bool b);
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);
	};

	class CINT : public ISmartType
	{
		friend class CSmartType;
		ssize_t m_value;
	public:
		CINT(ssize_t i);
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);
	};

	class CFLOAT : public ISmartType
	{
		friend class CSmartType;
		double m_value;
	public:
		CFLOAT(double d);
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);
	};

	class CSTRING : public ISmartType
	{
		friend class CSmartType;
		char* m_value;
	public:
		CSTRING(const char* str);
		~CSTRING();
		CSTRING &operator = (const CSTRING& str);
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);
	};

	class CLIST : public ISmartType
	{
		friend class CSmartType;
		vector<CSmartType> m_value;
	public:
		CSmartType& operator [] (size_t n);
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);
	};

	class CDICT : public ISmartType
	{
		friend class CSmartType;
		map<CString, CSmartType> m_value;
	public:
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);

		CSmartType& operator [] (const char* key);
	};

public:
	CSmartType();
	CSmartType(bool b);
	CSmartType(int i);
	CSmartType(double d);
	CSmartType(const char* str);
	~CSmartType();

	CSmartType(const CSmartType& e);
	CSmartType& operator = (const CSmartType& e);

	CSmartType& SmartInit(const CString &str);

	operator bool ();
	operator int ();
	operator ssize_t ();
	operator double ();
	operator const char* ();
	operator vector<CSmartType>& ();
	operator map<CString, CSmartType>& ();

	CSmartType& operator [] (const char* key);

	int ToStr(char* str, int n) const;
	CSmartType& ToList();
	CSmartType& ToDict();
};
