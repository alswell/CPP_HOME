#pragma once
#include "line2.h"
#include "line3.h"
#include "plain.h"
#include "rect.h"

#include "cycl.h"

namespace CG {

template <class T>
T Distance(const CVector2<T>& pt1, const CVector2<T>& pt2)
{
	T delta_x = pt1.x - pt2.x;
	T delta_y = pt1.y - pt2.y;
	return sqrt(T(SQUARE(delta_x) + SQUARE(delta_y)));
	//return sqrt((T)(SQUARE(pt1.x - pt2.x) + SQUARE(pt1.y - pt2.y)));
}

template<class T>
T Distance(const CVector3<T>& pt1, const CVector3<T>& pt2)
{
	T delta_x = pt1.x - pt2.x;
	T delta_y = pt1.y - pt2.y;
	T delta_z = pt1.z - pt2.z;
	return sqrt(T(SQUARE(delta_x) + SQUARE(delta_y) + SQUARE(delta_z)));
	//return sqrt((T)(SQUARE(pt1.x - pt2.x) + SQUARE(pt1.y - pt2.y) + SQUARE(pt1.z - pt2.z)));
}

template <class T>
T Distance(const CVector2<T>& pt1, T z1, const CVector2<T>& pt2, T z2)
{
	T delta_x = pt1.x - pt2.x;
	T delta_y = pt1.y - pt2.y;
	T delta_z = z1 - z2;
	return sqrt(T(SQUARE(delta_x) + SQUARE(delta_y) + SQUARE(delta_z)));
	//return sqrt((T)(SQUARE(pt1.x - pt2.x) + SQUARE(pt1.y - pt2.y) + SQUARE(z1 - z2)));
}

// distance of pt0 to line AB
template<class T>
T Distance(const CVector2<T>& pt0, const CVector2<T>& ptA, const CVector2<T>& ptB)
{
	CDirection<T> dir0(ptA, ptB);
	if (!dir0.IsValid())
		return Distance(pt0, ptA);

	CDirection<T> dir(ptA, pt0);
	if (!dir.IsValid())
		return 0;

	T delta = dir - dir0;
	T len = Distance(ptA, pt0);
	return (len * sin(delta));
}

template <class T>
T Area(const CVector2<T>& pt1, const CVector2<T>& pt2, const CVector2<T>& pt3)
{
	CDirection<T> dir1(pt1, pt2);
	if (!dir1.IsValid())
		return 0;
	CDirection<T> dir2(pt1, pt3);
	if (!dir2.IsValid())
		return 0;
	T delta = abs(dir1 - dir2);
	T len1 = Distance(pt1, pt2);
	T len2 = Distance(pt1, pt3);

	return (len1 * len2 * sin(delta)) / 2;
}

template <class T>
CVector2<T> Center(const CVector2<T>& pt1, const CVector2<T>& pt2, const CVector2<T>& pt3)
{
	return CVector2<T>((pt1.x + pt2.x + pt3.x) / 3, (pt1.y + pt2.y + pt3.y) / 3);
}

template <class T>
CVector2<T> Center(cycl<CVector2<T>> ls, float* pArea = nullptr)
{
	CVector2<T> ptCenter;
	float fArea = 0;
	typename cycl<CVector2<T>>::iterator itA = ls.entry();
	typename cycl<CVector2<T>>::iterator itB = +itA;
	typename cycl<CVector2<T>>::iterator itC = +itB;
	for (; itC != itA; ++itB, ++itC)
	{
		float s = Area(*itA, *itB, *itC);
		CVector2<float> c = Center(*itA, *itB, *itC);
		ptCenter.x += c.x * s;
		ptCenter.y += c.y * s;
		fArea += s;
	}
	ptCenter.x /= fArea;
	ptCenter.y /= fArea;
	if (pArea)
		*pArea = fArea;
	return ptCenter;
}

template<class T>
CVector2<T> CrossPoint(CLine2<T> line1, CLine2<T> line2)
{
	//T A, B;
	//(line1.m_pts[0].x - line1.m_pts[1].x) * A + line1.m_pts[0].x = (line2.m_pts[0].x - line2.m_pts[1].x) * B + line2.m_pts[0].x;
	//(line1.m_pts[0].y - line1.m_pts[1].y) * A + line1.m_pts[0].y = (line2.m_pts[0].y - line2.m_pts[1].y) * B + line2.m_pts[0].y;
	T l1xx = line1.m_pts[0].x - line1.m_pts[1].x;
	T l1yy = line1.m_pts[0].y - line1.m_pts[1].y;
	T l2xx = line2.m_pts[0].x - line2.m_pts[1].x;
	T l2yy = line2.m_pts[0].y - line2.m_pts[1].y;
	T l21xx = line2.m_pts[0].x - line1.m_pts[0].x;
	T l21yy = line2.m_pts[0].y - line1.m_pts[0].y;
	T B = (l21xx * l1yy - l21yy * l1xx) / (l2yy * l1xx - l2xx * l1yy);
	T A = (l2xx * B + l21xx) / l1xx;
	return CVector2<T>(l1xx * A + line1.m_pts[0].x, l1yy * A + line1.m_pts[0].y);
}

template<class T>
CVector3<T> CrossPoint(const CLine3<T>& seg, const CPointNormalPlain<T>& plain)
{
	CVector3<float> plain_v = plain.NoramlVector();
	T A = plain_v.x;
	T B = plain_v.y;
	T C = plain_v.z;
	CVector3<float> seg_v = seg.DirectionVector();
	T m = seg_v.x;
	T n = seg_v.y;
	T p = seg_v.z;

	T t = (A*(plain.m_M0.x - seg.m_pts[0].x) + B*(plain.m_M0.y - seg.m_pts[0].y) + C*(plain.m_M0.z - seg.m_pts[0].z)) / (A*m + B*n + C*p);

	return CVector3<T>(seg.m_pts[0].x + m * t, seg.m_pts[0].y + n * t, seg.m_pts[0].z + p * t);
}

template<class T>
bool PtInRect(const CRect<T>& rc, const CVector2<T>& pt)
{
	return pt.x >= rc.left && pt.x <= rc.right && pt.y >= rc.top && pt.y <= rc.bottom;
}

template<class T>
bool PtInRect(const CRect<T> &rc, int r, int c)
{
	return c >= rc.left && c < rc.right && r >= rc.top && r < rc.bottom;
}

}
