#ifndef __AFX_H
#define __AFX_H

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

#define FOR(cls, i, a, b) \
for (cls i = a; i < b; ++i)

#define FOR_LIST(cls, obj, it) \
for (list<cls>::iterator it = obj.begin(); it != obj.end(); ++it)

#define FOR_MAP(cls1, cls2, obj, it) \
for (map<cls1, cls2>::iterator it = obj.begin(); it != obj.end(); ++it)

#define FOR_I(a, b) \
for (int i = a; i < b; ++i)

#define DEF_INSTANCE(cls, ...) \
cls* cls::Instance()\
{\
	static cls* pInstance = NULL;\
	if (!pInstance)\
		pInstance = new cls(__VA_ARGS__);\
	return pInstance;\
}

#endif
