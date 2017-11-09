#include "json.h"

void JsonDump(CStr& str, DICT(CStr) dict)
{
	str = "{";
	FOR_DICT(CStr, dict, it)
	{
		str.AppendFormat("\"%s\": \"%s\", ", (const char*)it->first, (const char*)it->second);
	}
	str.TrimRight()[-1] = '}';
}
