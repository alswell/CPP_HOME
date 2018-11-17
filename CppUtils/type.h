#pragma once

#include "env.h"
//#include "dict.h"
#include "str.h"

#define REF_LIST vector<CSmartType>&
#define REF_DICT map<CString, CSmartType>&

class CSmartType
{
	class ISmartType
	{
	public:
		virtual ~ISmartType();
		virtual ISmartType* Copy() = 0;
		virtual int ToStr(char* str, int n) = 0;
		virtual operator bool () = 0;
	};
	ISmartType* m_pSmartTypeImpl;

	class SmtBool : public ISmartType
	{
		friend class CSmartType;
		bool m_value;
	public:
		SmtBool(bool b);
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);
		virtual operator bool ();
	};

	class SmtInt : public ISmartType
	{
		friend class CSmartType;
		ssize_t m_value;
	public:
		SmtInt(ssize_t i);
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);
		virtual operator bool ();
	};

	class SmtFloat : public ISmartType
	{
		friend class CSmartType;
		double m_value;
	public:
		SmtFloat(double d);
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);
		virtual operator bool ();
	};

	class SmtStr : public ISmartType
	{
		friend class CSmartType;
		char* m_value;
	public:
		SmtStr(const char* str);
		~SmtStr();
		SmtStr &operator = (const SmtStr& str);
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);
		virtual operator bool ();
	};

	class SmtList : public ISmartType
	{
		friend class CSmartType;
		vector<CSmartType> m_value;
	public:
		CSmartType& operator [] (size_t n);
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);
		virtual operator bool ();
	};

	class SmtDict : public ISmartType
	{
		friend class CSmartType;
		map<CString, CSmartType> m_value;
	public:
		virtual ISmartType* Copy();
		virtual int ToStr(char* str, int n);
		virtual operator bool ();

		CSmartType& operator [] (const char* key);
	};

public:
	CString m_strBuff;
	CSmartType();
	CSmartType(bool b);
	CSmartType(int i);
	CSmartType(double d);
	CSmartType(const char* str);
	static const CSmartType& GetEmptyList();
	static const CSmartType& GetEmptyDict();
	~CSmartType();

	CSmartType(const CSmartType& e);
	CSmartType& operator = (const CSmartType& e);
	bool operator ==(const CSmartType& e);
	bool operator !=(const CSmartType& e);

	CSmartType& SmartInit(const CString &str);

	operator bool ();
	operator int ();
	operator ssize_t ();
	operator double ();
	operator char* ();
	operator const char* ();

	operator REF_LIST ();
	operator REF_DICT ();

	CSmartType &operator [](const char* key);
	CSmartType &operator [](int i);
	CSmartType &operator [](const CSmartType& key);

	int ToStr(char* str, int n) const;
};
extern CSmartType NONE;
#define L0 CSmartType::GetEmptyList()
#define D0 CSmartType::GetEmptyDict()


