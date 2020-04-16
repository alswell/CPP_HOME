#pragma once
#ifdef WIN32
#include <atltypes.h>
#endif
#include "env.h"
#include "math.h"

#ifndef __device__ 
#define __device__ 
#endif

template <class T>
class CVector2
{
public:
	T x;
	T y;
	//bool m_bValid;

	__device__ CVector2() : x(0), y(0) {}
	__device__ CVector2(const T& _x, const T& _y) : x(_x), y(_y) {}
#ifdef WIN32
	CVector2(const CPoint& pt) : x(pt.x), y(pt.y) {}
#endif

	template <class DES_T>
	operator CVector2<DES_T>() const
	{
		CVector2<DES_T> dest_p;
		dest_p.x = static_cast<DES_T>(x);
		dest_p.y = static_cast<DES_T>(y);
		return dest_p;
	}
	operator T() const
	{
		return sqrt(SQUARE(x) + SQUARE(y));
	}
	bool operator == (CVector2<T> v) const
	{
		return x == v.x && y == v.y;
	}

	bool operator != (CVector2<T> pt) const
	{
		return !(*this == pt);
	}

	bool operator () (T _x, T _y) const
	{
		return x == _x && y == _y;
	}

	CVector2<T> operator- () const
	{
		return CVector2<T>(-x, -y);
	}

	CVector2<T>& Offset(T _x, T _y)
	{
		x += _x;
		y += _y;
		return *this;
	}
	CVector2<T>& operator += (const CVector2<T>& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	CVector2<T> operator + (const CVector2<T>& v) const
	{
		CVector2<T> r = *this;
		r += v;
		return r;
	}
	CVector2<T>& operator -= (const CVector2<T>& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	CVector2<T> operator - (const CVector2<T>& v) const
	{
		CVector2<T> r = *this;
		r -= v;
		return r;
	}
	T operator * (const CVector2<T>& v) const //dotProduct
	{
		return x * v.x + y * v.y;
	}
	T operator / (const CVector2<T>& v) const //crossProduct
	{
		return y * v.x - x * v.y;
	}

	CVector2<T>& operator += (T n)
	{
		x += n;
		y += n;
		return *this;
	}
	CVector2<T> operator + (T n) const
	{
		CVector2<T> r = *this;
		r += n;
		return r;
	}
	CVector2<T>& operator -= (T n)
	{
		x -= n;
		y -= n;
		return *this;
	}
	CVector2<T> operator - (T n) const
	{
		CVector2<T> r = *this;
		r -= n;
		return r;
	}
	CVector2<T>& operator *= (T n)
	{
		x *= n;
		y *= n;
		return *this;
	}
	CVector2<T> operator * (T n) const
	{
		CVector2<T> r = *this;
		r *= n;
		return r;
	}
	CVector2<T>& operator /= (T n)
	{
		x /= n;
		y /= n;
		return *this;
	}
	CVector2<T> operator / (T n) const
	{
		CVector2<T> r = *this;
		r /= n;
		return r;
	}

	T Len()
	{
		return sqrt(SQUARE(x) + SQUARE(y));
	}

	__device__ void RotationSelf(T delta)
	{
		//T x = pt.x * cosf(delta) - pt.y * sinf(delta);
		//T y = pt.x * sinf(delta) + pt.y * cosf(delta);
		//return CPointEx<T>(x, y);
		T sin_delta = sinf(delta);
		T cos_delta = cosf(delta);
		T temp_x = x * cos_delta - y * sin_delta;
		y = x * sin_delta + y * cos_delta;
		x = temp_x;
	}
	__device__ CVector2<T> Rotation(T delta) const
	{
		//T x = pt.x * cosf(delta) - pt.y * sinf(delta);
		//T y = pt.x * sinf(delta) + pt.y * cosf(delta);
		//return CPointEx<T>(x, y);
		T sin_delta = sinf(delta);
		T cos_delta = cosf(delta);
		return CVector2<T>(x * cos_delta - y * sin_delta, x * sin_delta + y * cos_delta);
	}
	CVector2<T> Rotation(CVector2<T> pt0, T delta) const
	{
		CVector2<T> pt = *this;
		pt -= pt0;
		pt = pt.Rotation(delta);
		pt += pt0;
		return pt;
	}
	//CPointEx<T> Rotation()
	//{
	//	return Rotation((T)-CDirection<T>(CPointEx<T>(0, 0), *this));
	//}

	//template<class T>
	//CPointEx<T> Rotation(CPointEx<T> pt0)
	//{
	//	CPointEx<T> pt = *this;
	//	pt -= pt0;
	//	pt = pt.Rotation();
	//	pt += pt0;
	//	return pt;
	//}
	CVector2<T> Clockwise90() const
	{
		return CVector2<T>(y, -x);
	}
	CVector2<T> Anticlockwise90() const
	{
		return CVector2<T>(-y, x);
	}
};

template <class T>
const char* ToStr(CVector2<T> v)
{
//	static char buff[128];
//	sprintf(buff, "[%s, %s]", _(v.x), _(v.y));
//	return buff;
	return SPRINTF("[%s, %s]", _(v.x), _(v.y));
}

typedef CVector2<int> POINT;
typedef CVector2<float> PointF;


template <class T>
class Coords;
template <class T>
class RowCol;

template <class T>
class Point: public CVector2<T>
{
public:
	template <class SRC_T>
	Point(CVector2<SRC_T> pt) : CVector2<T>(pt) {}
	Point(Coords<T> xy) : CVector2<T>(xy.x, -xy.y) {}
	Point(RowCol<T> rc) : CVector2<T>(rc.y, rc.x) {}

	void Up(int i) { CVector2<T>::y -= i; }
	void Left(int i) { CVector2<T>::x -= i; }
};

template <class T>
class RowCol: public CVector2<T>
{
public:
	RowCol(T r, T c) : CVector2<T>(r, c) {}
	template <class SRC_T>
	RowCol(CVector2<SRC_T> pt) : CVector2<T>(pt) {}
	RowCol(Coords<T> xy) : CVector2<T>(-xy.y, xy.x) {}
	RowCol(Point<T> pt) : CVector2<T>(pt.y, pt.x) {}

	void Up(int i) { CVector2<T>::x -= i; }
	void Left(int i) { CVector2<T>::y -= i; }
};

template <class T>
class Coords: public CVector2<T>
{
public:
	template <class SRC_T>
	Coords(CVector2<SRC_T> pt) : CVector2<T>(pt) {}
	Coords(Point<T> pt) : CVector2<T>(pt.x, -pt.y) {}
	Coords(RowCol<T> rc) : CVector2<T>(rc.y, -rc.x) {}

	void Up(int i) { CVector2<T>::y += i; }
	void Left(int i) { CVector2<T>::x -= i; }
};

template <class T>
CVector2<T>* GetIterItem(CVector2<T>* pt)
{
	return pt;
}
template <class T>
CVector2<T>* GetIterItem(CVector2<T>& pt)
{
	return &pt;
}

