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
template<class T>
T Distance(const CVector2<T>& pt0, CLine2<T>& line)
{
	return Distance(pt0, line.m_pts[0], line.m_pts[1]);
}
// vector 0A's projection on AB
template<class T>
T Projection(const CVector2<T>& pt0, const CVector2<T>& ptA, const CVector2<T>& ptB)
{
	CDirection<T> dir0(ptA, ptB);
	if (!dir0.IsValid())
		return Distance(pt0, ptA);

	CDirection<T> dir(ptA, pt0);
	if (!dir.IsValid())
		return 0;

	T delta = dir - dir0;
	T len = Distance(ptA, pt0);
	return (len * cos(delta));
}

template<class T>
T Distance(const CVector2<T>& pt, const CRay2<T>& ray)
{
	if (!ray.m_dir.IsValid())
		return Distance(ray.m_pt0, pt);

	CDirection<T> dir(ray.m_pt0, pt);
	if (!dir.IsValid())
		return 0;

	T delta = dir - ray.m_dir;
	T len = Distance(ray.m_pt0, pt);
	return (len * sin(delta));
}
template<class T>
T Projection(const CVector2<T>& pt, const CRay2<T>& ray)
{
	if (!ray.m_dir.IsValid())
		return Distance(ray.m_pt0, pt);

	CDirection<T> dir(ray.m_pt0, pt);
	if (!dir.IsValid())
		return 0;

	T delta = dir - ray.m_dir;
	T len = Distance(ray.m_pt0, pt);
	return (len * cos(delta));
}

template <class T>
CVector3<T> MidPoint(const CVector3<T>& pt1, const CVector3<T>& pt2)
{
	return CVector3<T>((pt1.x + pt2.x)/2, (pt1.y + pt2.y)/2, (pt1.z + pt2.z)/2);
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
const CLine3<T> CommonPerpendicular(const CLine3<T>& segA, const CLine3<T>& segB)
{
	auto vA = segA.DirectionVector();
	auto vB = segB.DirectionVector();

	auto a1 = segA.A.x - segA.B.x;
	auto a2 = segA.A.y - segA.B.y;
	auto a3 = segA.A.z - segA.B.z;
	auto b1 = segB.A.x - segB.B.x;
	auto b2 = segB.A.y - segB.B.y;
	auto b3 = segB.A.z - segB.B.z;
	auto c1 = segA.B.x - segB.B.x;
	auto c2 = segA.B.y - segB.B.y;
	auto c3 = segA.B.z - segB.B.z;

	auto A1 = a1 * vA.x + a2 * vA.y + a3 * vA.z;
	auto B1 = b1 * vA.x + b2 * vA.y + b3 * vA.z;
	auto C1 = c1 * vA.x + c2 * vA.y + c3 * vA.z;
	auto A2 = a1 * vB.x + a2 * vB.y + a3 * vB.z;
	auto B2 = b1 * vB.x + b2 * vB.y + b3 * vB.z;
	auto C2 = c1 * vB.x + c2 * vB.y + c3 * vB.z;

	auto b = (C2 * A1 - C1 * A2) / (B2 * A1 - B1 * A2);
	auto a = (b * B1 - C1) / A1;
	CVector3<T> ptA(a * a1 + segA.B.x, a * a2+segA.B.y, a * a3+segA.B.z);
	CVector3<T> ptB(b * b1 + segB.B.x, b * b2+segB.B.y, b * b3+segB.B.z);
	return CLine3<T>(ptA, ptB);
}

template<class T>
CVector3<T> CrossPoint(const CLine3<T>& segA, const CLine3<T>& segB)
{
	auto seg = CommonPerpendicular(segA, segB);
	return MidPoint(seg.A, seg.B);
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
