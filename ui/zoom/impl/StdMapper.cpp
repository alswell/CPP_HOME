#include "StdMapper.h"

void CStdMapper::Map(float& x, float& y)
{
	if (m_nMulti > 0)
	{
		x = x / m_nMulti;
		y = y / m_nMulti;
	}
	else
	{
		x = x * -m_nMulti;
		y = y * -m_nMulti;
	}
}

void CStdMapper::Revert(float& x, float& y)
{
	if (m_nMulti > 0)
	{
		x = x * m_nMulti;
		y = y * m_nMulti;
	}
	else
	{
		x = x / -m_nMulti;
		y = y / -m_nMulti;
	}
}
