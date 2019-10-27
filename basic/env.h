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
#include <math.h>

#include <vector>
#include <list>
#include <map>

using namespace std;

//#ifndef CONF_THIS_CLS
//#define CONF_THIS_CLS(cls) typedef cls THIS_CLS
//#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define PTXY(pt) pt.x, pt.y
#define PTXY_INT(pt) int(pt.x), int(pt.y)
#define PTXYFLOAT(pt) float(pt.x), float(pt.y)
#define PTYX(pt) pt.y, pt.x
#define PTYX_INT(pt) int(pt.y), int(pt.x)
#define PTYX_FLOAT(pt) float(pt.y), float(pt.x)

#ifndef DWORD
#define DWORD long
#endif

#define FOR(cls, i, a, b) \
	for (cls i = a; i < b; ++i)

#define FOR_EACH(it, obj) \
	for (auto it = (obj).begin(); it != (obj).end(); ++it)

#define FOR_LIST(cls, obj, it) \
	for (typename list<cls>::iterator it = obj.begin(); it != obj.end(); ++it)

#define FOR_MAP(cls1, cls2, obj, it) \
	for (typename map<cls1, cls2>::iterator it = obj.begin(); it != obj.end(); ++it)

#define FOR_(a, b) \
	for (int i = a; i < b; ++i)
#define FOR_I(a, i, b) \
	for (int i = int(a); i < int(b); ++i)

#define FOR_IJ(i_beg, i, i_end, j_beg, j, j_end) \
	for (int i = i_beg; i < i_end; ++i)\
	for (int j = j_beg; j < j_end; ++j)

#define FOR_U(a, i, b) \
	for (unsigned i = a; i < b; ++i)
#define FOR_SIZE(a, i, b) \
	for (size_t i = a; i < b; ++i)

#define FOR_MX(mx, r, c) \
	for (int r = 0; r < int((mx).m_uRow); ++r)\
	for (int c = 0; c < int((mx).m_uColumn); ++c)

#define FOR_SQUARE(i_beg, i, i_end, j_beg, j, j_end) \
	for (int i = i_beg; i <= i_end; ++i)\
	for (int j = j_beg; j <= j_end; ++j)

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

//template <class T>
//struct SVectorP
//{
//	vector<T*> v;
//	~SVectorP()	{ FOR_EACH(it, v) delete *it; }
//	void Detach(vector<T*>& x) { x.clear(); x = v; v.clear(); }
//};
//template <class T>
//struct SListP
//{
//	list<T*> l;
//	~SListP()	{ FOR_EACH(it, l) delete *it; }
//	void Detach(list<T*>& x) { x.clear(); x = l; l.clear(); }
//};
//template <class T>
//struct SPointer
//{
//	T* p;
//	SPointer(T* x = nullptr) : p(x) {}
//	~SPointer()	{ if (p) delete p; }
//	T* Detach() { T* x = p; p = nullptr; return x; }
//};

template <class T>
list<T> VectorToList(vector<T> v)
{
	list<T> l;
	FOR_EACH(it, v)
		l.push_back(*it);
	return l;
}
template <class T>
vector<T> ListToVector(list<T> l)
{
	vector<T> v;
	FOR_EACH(it, l)
		v.push_back(*it);
	return v;
}

#define DEC_INSTANCE(cls) static cls* Instance()

#define DEF_INSTANCE(cls, ...) \
cls* cls::Instance()\
{\
	static cls* pInstance = NULL;\
	if (!pInstance)\
		pInstance = new cls(__VA_ARGS__);\
	return pInstance;\
}

// math macro
#define PI       3.14159265358979323846
#define PI_2     1.57079632679489661923
#define PI_3     1.047197551196597746153
#define PI_4     0.785398163397448309616
#define PI_8     0.3926990816987241
#define PI_D     6.28318530717958647692
#define SQRT_2   1.414213562373095048802
#define PIf       3.14159265358979323846f
#define PI_2f     1.57079632679489661923f
#define PI_3f     1.047197551196597746153f
#define PI_4f     0.785398163397448309616f
#define PI_8f     0.3926990816987241f
#define PI_Df     6.28318530717958647692f
#define SQRT_2f   1.414213562373095048802f
#define CUBIC(x) ((x) * (x) * (x))
#define SQUARE(x) ((x) * (x))
