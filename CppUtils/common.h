#ifndef __COMMON_H
#define __COMMON_H

#define FOR(cls, i, a, b) \
for (cls i = a; i < b; ++i) 

#define FOR_LIST(cls, obj, it) \
for (list<cls>::iterator it = obj.begin(); it != obj.end(); ++it) 

#define FOR_MAP(cls1, cls2, obj, it) \
for (map<cls1, cls2>::iterator it = obj.begin(); it != obj.end(); ++it) 

#define FOR_I(a, b) \
for (int i = a; i < b; ++i) 

#endif
