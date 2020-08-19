#pragma once
#include "direction.h"


#define ACCURACY 1000
template <class T>
class CLine2
{
public:
	CVector2<T> A, B;
	CLine2() {}
	CLine2(const CVector2<T>& pt1, const CVector2<T>& pt2)
	{
		A = pt1;
		B = pt2;
	}
	CLine2(CVector2<T> pts[])
	{
		A = pts[0];
		B = pts[1];
	}
	CLine2(const CVector2<T>& pt1, const T k)
	{
		A = pt1;
		B.x = A.x + ACCURACY;
		B.y = A.y + ACCURACY * k;
	}
	CLine2(const CVector2<T>& pt1, const CDirection<T>& dir)
	{
		A = pt1;
		//T k = tan((T)dir);
		T k = dir.y / dir.x;
		B.x = A.x + ACCURACY;
		B.y = A.y + ACCURACY * k;
	}
	operator bool ()
	{
		return A != B;
	}

	T k()
	{
		return (B.y - A.y) / (B.x - A.x);
	}
	T b()
	{
		return A.y - k() * A.x;
	}
	CDirection<T> Direction()
	{
		return CDirection<T>(A, B);
	}
	bool IsVertical()
	{
		if (A.x == B.x)
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
	CRay2(const CVector2<T>& pt0, const CVector2<T>& pt1, const CVector2<T>& pt2, T rotation = 0)
	{
		m_pt0 = pt0;
		m_dir.SetValue(pt1, pt2);
		m_dir += rotation;
	}
	CRay2(const CVector2<T>& pt1, const CVector2<T>& pt2, T rotation = 0)
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
