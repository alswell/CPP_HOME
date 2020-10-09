#include "dict.h"

size_t _str_hash::operator ()(const CString &str) const
{
	return _hash((const char*)str);
}
