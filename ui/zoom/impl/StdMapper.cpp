#include "StdMapper.h"

void CStdMapper::Map(float& x, float& y)
{
	x = x * m_nMultiD / m_nMulti;
	y = y * m_nMultiD / m_nMulti;
}

void CStdMapper::Revert(float& x, float& y)
{
	x = x * m_nMulti / m_nMultiD;
	y = y * m_nMulti / m_nMultiD;
}
