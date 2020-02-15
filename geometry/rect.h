#pragma once
#include "vector2.h"
template <class T>
class CRect
{
public:
	T left;
	T top;
	T right;
	T bottom;
	CRect() : left(0), top(0), right(0), bottom(0) {}
	CRect(T l, T t, T r, T b) : left(l), top(t), right(r), bottom(b) {}
	T Width() const { return right - left; }
	T Height() const { return bottom - top; }
	bool IsRectEmpty() const { return (right - left <= 0) || (bottom - top <= 0 ); }
	bool IsPtInside(CVector2<T> pt) const { return pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom; }
	void OffsetRect(T x, T y) { left += x; right += x; top += y; bottom += y; }
	void MoveToXY(T x, T y) { right += x - left; left = x; bottom += y - top; top = y; }
	void MoveToX(T x) { right += x - left; left = x; }
	void MoveToY(T y) { bottom += y - top; top = y; }
	void OffsetRect(CVector2<T> pt) { left += pt.x; right += pt.x; top += pt.y; bottom += pt.y; }
	void IntersectRect(const CRect& rc1, const CRect& rc2)
	{
		left = rc1.left > rc2.left ? rc1.left : rc2.left;
		right = rc1.right < rc2.right ? rc1.right : rc2.right;
		top = rc1.top > rc2.top ? rc1.top : rc2.top;
		bottom = rc1.bottom < rc2.bottom ? rc1.bottom : rc2.bottom;
	}
	void UnionRect(const CRect& rc1, const CRect& rc2)
	{
		left = rc1.left < rc2.left ? rc1.left : rc2.left;
		right = rc1.right > rc2.right ? rc1.right : rc2.right;
		top = rc1.top < rc2.top ? rc1.top : rc2.top;
		bottom = rc1.bottom > rc2.bottom ? rc1.bottom : rc2.bottom;
	}
	void SetRectEmpty()
	{
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
	}

	template<class T2>
	void operator *= (T2 x)
	{
		left *= x;
		top *= x;
		right *= x;
		bottom *= x;
	}
};

template <class T>
const char* ToStr(CRect<T> rc)
{
//	static char buff[128];
//	sprintf(buff, "[%s, %s]", _(v.x), _(v.y));
//	return buff;
	return SPRINTF("{(%s, %s); [%s * %s]}", _(rc.left), _(rc.top), _(rc.Width()), _(rc.Height()));
}

typedef CRect<int> RECT;

template <class T>
class CRectLimit : public CRect<T>
{
public:
	CRectLimit(T r, T c) : CRect<T>(c, r, c, r) {}
	void Visit(T r, T c)
	{
		if (CRect<T>::left > c)
			CRect<T>::left = c;
		else if (CRect<T>::right < c)
			CRect<T>::right = c;
		if (CRect<T>::top > r)
			CRect<T>::top = r;
		else if (CRect<T>::bottom < r)
			CRect<T>::bottom = r;
	}
	CVector2<T> Base()
	{
		return CVector2<T>(CRect<T>::top, CRect<T>::left);
	}
};
