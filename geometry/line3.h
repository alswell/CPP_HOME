#pragma once
#include "vector3.h"


template <class T>
class CLine3
{
public:
	CVector3<T> A, B;
	CLine3(const CVector3<T>& pt1, const CVector3<T>& pt2)
	{
		A = pt1;
		B = pt2;
	}

	int m_bIsZValid;
	T Z(T x, T y)
	{
		if (B.x == A.x)
			return (B.z - A.z) / (B.y - A.y) * (y - A.y) + A.z;
		else
			return (B.z - A.z) / (B.x - A.x) * (x - A.x) + A.z;
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
		return CVector3<T>(B.x - A.x, B.y - A.y, B.z - A.z);
	}
};
