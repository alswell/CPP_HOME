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
	template<class T2>
	CRect(const CRect<T2> rc) : left(rc.left), top(rc.top), right(rc.right), bottom(rc.bottom) {}
	T Width() const { return right - left; }
	T Height() const { return bottom - top; }
	void SetRectEmpty() { left = right = top = bottom = 0; }
	bool IsRectEmpty() const { return (right - left <= 0) || (bottom - top <= 0 ); }
	bool IsPtInside(CVector2<T> pt) const { return pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom; }
	void OffsetRect(T x, T y) { left += x; right += x; top += y; bottom += y; }
	void MoveToXY(T x, T y) { right += x - left; left = x; bottom += y - top; top = y; }
	void MoveToX(T x) { right += x - left; left = x; }
	void MoveToY(T y) { bottom += y - top; top = y; }
	void OffsetRect(CVector2<T> pt) { left += pt.x; right += pt.x; top += pt.y; bottom += pt.y; }
	void operator *= (const CRect& rc)
	{
		left = left > rc.left ? left : rc.left;
		right = right < rc.right ? right : rc.right;
		top = top > rc.top ? top : rc.top;
		bottom = bottom < rc.bottom ? bottom : rc.bottom;
	}
	void operator += (const CRect& rc)
	{
		left = left < rc.left ? left : rc.left;
		right = right > rc.right ? right : rc.right;
		top = top < rc.top ? top : rc.top;
		bottom = bottom > rc.bottom ? bottom : rc.bottom;
	}
	template<class T2>
	void operator *= (T2 x) { left *= x; top *= x; right *= x; bottom *= x; }
};

template <class T>
void Output(CRect<T> rc)
{
	Printf("{(%s, %s) %s*%s}", rc.left, rc.top, rc.Width(), rc.Height());
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
