#pragma once
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <unistd.h>
#include <cerrno>
#include <iostream>

#include <malloc.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

#include <vector>
#include <list>
#include <map>

using namespace std;

#define V2A(v) (&(v[0]))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))


#ifndef DWORD
#define DWORD long
#endif

#define FOR(cls, i, a, b) \
for (cls i = a; i < b; ++i)

#define FOR_LIST(cls, obj, it) \
for (typename list<cls>::iterator it = obj.begin(); it != obj.end(); ++it)

#define FOR_MAP(cls1, cls2, obj, it) \
for (typename map<cls1, cls2>::iterator it = obj.begin(); it != obj.end(); ++it)

#define FOR_I(a, b) \
for (int i = a; i < b; ++i)

#define FOR_MX(mx, r, c) \
for (int r = 0; r < (mx).m_uRow; ++r)\
for (int c = 0; c < (mx).m_uColumn; ++c)

#define FOR_SQUARE(i, i_beg, i_end, j, j_beg, j_end) \
for (int i = i_beg; i < i_end; ++i)\
for (int j = j_beg; j < j_end; ++j)

#define VA_HELPER(first, func) \
va_list ap;\
va_start(ap, first);\
(func)(ap);\
va_end(ap)

template<class T>
struct LS
{
	vector<T> v;
	T* buff;
	unsigned size;
	LS(unsigned n, T* l=NULL)
	{
		size = n;
		if (l)
		{
			buff = l;
		}
		else
		{
			v.resize(size);
			buff = &v[0];
		}
	}
	LS& operator() (...)
	{
		VA_HELPER(this, *this);
		return *this;
	}
	void operator() (va_list ap)
	{
		for (unsigned i = 0; i < size; ++i)
			buff[i] = va_arg(ap, T);
	}
	operator T*()
	{
		return buff;
	}
};

#define DEC_INSTANCE(cls) static cls* Instance()

#define DEF_INSTANCE(cls, ...) \
cls* cls::Instance()\
{\
	static cls* pInstance = NULL;\
	if (!pInstance)\
		pInstance = new cls(__VA_ARGS__);\
	return pInstance;\
}
