#pragma once
#include "vector3.h"


template <class T>
class CLine3
{
public:
	CVector3<T> m_pts[2];
	CVector3<T>& A;
	CVector3<T>& B;
	CLine3(const CVector3<T>& pt1, const CVector3<T>& pt2)
		: A(m_pts[0])
		, B(m_pts[1])
	{
		m_pts[0] = pt1;
		m_pts[1] = pt2;
	}

	int m_bIsZValid;
	T Z(T x, T y)
	{
		if (m_pts[1].x == m_pts[0].x)
			return (m_pts[1].z - m_pts[0].z) / (m_pts[1].y - m_pts[0].y) * (y - m_pts[0].y) + m_pts[0].z;
		else
			return (m_pts[1].z - m_pts[0].z) / (m_pts[1].x - m_pts[0].x) * (x - m_pts[0].x) + m_pts[0].z;
	}
	T Z(CVector2<T> pt)
	{
		return Z(pt.x, pt.y);
	}
	//T Z(T y) {
	//	return (pt2.z - pt1.z) / (pt2.y - pt1.y)*(y - pt1.y) + pt1.z;
	//}
	//T Z(CPointEx pt)
	//{
	//	//return Z(pt.x, pt.y);
	//}

	CVector3<T> DirectionVector() const
	{
		return CVector3<T>(m_pts[1].x - m_pts[0].x, m_pts[1].y - m_pts[0].y, m_pts[1].z - m_pts[0].z);
	}
};
