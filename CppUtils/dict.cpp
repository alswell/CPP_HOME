#include "dict.h"

size_t _str_hash::operator ()(CStr str) const
{
	return _hash((const char*)str);
}
