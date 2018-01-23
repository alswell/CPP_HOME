#ifndef __JSON_H
#define __JSON_H
#include "dict.h"
#include "type.h"

namespace JSON {
	void Dump(CStr& str, DICT(CStr) dict);

	void Load(CValueType& json, const char* str);
	char *ParseDict(CValueType& dict, const char* str);
	char *ParseList(CValueType& list, const char* str);
}

#endif
