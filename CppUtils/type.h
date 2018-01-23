#ifndef __TYPE_H
#define __TYPE_H

#include "afx.h"
#include "dict.h"

enum EValueType
{
	TYPE_UNKNOWN = 0,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_BOOL,

	TYPE_STRING,
	TYPE_LIST,
	TYPE_DICT,
};

class CValueType
{
public:
	EValueType m_type;
	void* m_value;

	CValueType(EValueType t = TYPE_UNKNOWN, void* v = NULL);
	~CValueType();

	CValueType(const CValueType &v);

	ssize_t GetInt() const;
	void SetInt(ssize_t i);

	double GetFloat() const;
	void SetFloat(double d);

	bool GetBool() const;
	void SetBool(bool b);

	char *GetString() const;
	void SetString(const char* str);

	vector<CValueType> &GetList() const;
	void SetList();

	CDict<CValueType> &GetDict() const;
	void SetDict();

	void SetStrInt(const CStr& str);

private:
	void Release();
};

#endif
