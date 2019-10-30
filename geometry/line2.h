#pragma once
#include "direction.h"


#define ACCURACY 1000
template <class T>
class CLine2
{
public:
	CVector2<T> m_pts[2];
	CLine2() {}
	CLine2(const CVector2<T>& pt1, const CVector2<T>& pt2)
	{
		m_pts[0] = pt1;
		m_pts[1] = pt2;
	}
	CLine2(CVector2<T> pts[])
	{
		m_pts[0] = pts[0];
		m_pts[1] = pts[1];
	}
	CLine2(const CVector2<T>& pt1, const T k)
	{
		m_pts[0] = pt1;
		m_pts[1].x = m_pts[0].x + ACCURACY;
		m_pts[1].y = m_pts[0].y + ACCURACY * k;
	}
	CLine2(const CVector2<T>& pt1, const CDirection<T>& dir)
	{
		m_pts[0] = pt1;
		//T k = tan((T)dir);
		T k = dir.y / dir.x;
		m_pts[1].x = m_pts[0].x + ACCURACY;
		m_pts[1].y = m_pts[0].y + ACCURACY * k;
	}
	bool IsValid()
	{
		return m_pts[0] != m_pts[1];
	}

	T k()
	{
		return (m_pts[1].y - m_pts[0].y) / (m_pts[1].x - m_pts[0].x);
	}
	T b()
	{
		return m_pts[0].y - k() * m_pts[0].x;
	}
	CDirection<T> Direction()
	{
		return CDirection<T>(m_pts[0], m_pts[1]);
	}
	bool IsVertical()
	{
		if (m_pts[0].x == m_pts[1].x)
			return true;
		return false;
	}
};

template <class T>
class CRay2
{
public:
	CVector2<T> m_pt0;
	CDirection<T> m_dir;
	CRay2(const CVector2<T>& pt1, const CVector2<T>& pt2)
	{
		m_pt0 = pt1;
		m_dir.SetValue(pt1, pt2);
	}
	CRay2(const CVector2<T>& pt1, const CVector2<T>& pt2, T rotation)
	{
		m_pt0 = pt1;
		m_dir.SetValue(pt1, pt2);
		m_dir += rotation;
	}
	CVector2<T> Endpoint(T accuracy) const
	{
		return m_pt0 + m_dir.Vector(accuracy);
	}
	CLine2<T> Line(T len) const
	{
		return CLine2<T>(m_pt0, m_pt0 + m_dir.Vector(len));
	}
	void Rotate(T delta)
	{
		m_dir += delta;
	}
};
