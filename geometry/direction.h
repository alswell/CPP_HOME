#pragma once
#include "vector2.h"

#define INVALID_DIRECTION -444

#define SIMPLE_DIR

#ifdef SIMPLE_DIR

template <class T>
class CDirection
{
	//friend class CScanRange;
	T m_Direction;
	bool m_bValid;
public:
	__device__ static T Normalization(T direction)
	{
		while (direction <= -PI)
			direction += PI_D;
		while (direction >= PI)
			direction -= PI_D;
		return direction;
	}
	__device__ CDirection() : m_Direction(INVALID_DIRECTION), m_bValid(false) {}
	__device__ CDirection(T direction) { m_Direction = Normalization(direction); m_bValid = true; }

	template <class VAL_T>
	__device__ CDirection(VAL_T x, VAL_T y)
	{
		SetValue(CVector2<VAL_T>(0, 0), CVector2<VAL_T>(x, y));
	}

	template <class VAL_T>
	__device__ CDirection(const CVector2<VAL_T>& pt)
	{
		SetValue(CVector2<VAL_T>(0, 0), pt);
	}

	template <class VAL_T>
	__device__ CDirection(const CVector2<VAL_T>& pt1, const CVector2<VAL_T>& pt2)
	{
		SetValue(pt1, pt2);
	}

	template <class VAL_T>
	__device__ void SetValue(const CVector2<VAL_T>& pt1, const CVector2<VAL_T>& pt2)
	{
		m_bValid = true;
		m_Direction = INVALID_DIRECTION;
		if (pt2.x == pt1.x)
		{
			if (pt2.y == pt1.y)
				m_bValid = false;
			else if (pt2.y > pt1.y)
				m_Direction = PI_2;
			else
				m_Direction = -PI_2;
		}
		else
		{
			m_Direction = atan((T)(pt2.y - pt1.y) / (T)(pt2.x - pt1.x));
			if (pt2.x - pt1.x < 0)
			{
				if (m_Direction > 0)
					m_Direction = m_Direction - PI;
				else
					m_Direction = m_Direction + PI;
			}
		}
	}

	inline bool IsValid() const { return m_bValid; }

	operator T() const { return m_Direction; }

	// 运算：
	CDirection<T>& operator= (T direction)
	{
		m_Direction = Normalization(direction);
		m_bValid = true;
		return *this;
	}
	CDirection<T> operator- () const
	{
		return CDirection<T>(-m_Direction);
	}

	CDirection<T> operator- (T dir) const
	{
		return CDirection<T>(Normalization(m_Direction - dir));
	}

	CDirection<T>& operator-= (T dir)
	{
		m_Direction = Normalization(m_Direction - dir);
		return *this;
	}

	__device__ T operator- (const CDirection<T>& direction) const
	{
		return Normalization(m_Direction - direction.m_Direction);
	}

	CDirection<T> operator+ (T dir) const
	{
		return CDirection<T>(Normalization(m_Direction + dir));
	}

	CDirection<T>& operator+= (T dir)
	{
		m_Direction = Normalization(m_Direction + dir);
		return *this;
	}

	CDirection<T> operator+ (const CDirection<T>& direction) const
	{
		T tmp = (direction - *this) / 2;
		CDirection<T> r(*this + tmp);
		if (tmp < 0)
			r += PI;
		return r;
	}

	CDirection<T>& operator+= (const CDirection<T>& direction)
	{
		T tmp = (direction - *this) / 2;
		*this += tmp;
		if (tmp < 0)
			*this += PI;
		return *this;
	}

	bool operator== (const CDirection<T>& direction) const
	{
		return m_Direction == direction.m_Direction;
	}
	bool operator!= (const CDirection<T>& direction) const
	{
		return !(m_Direction == direction.m_Direction);
	}
	bool operator< (const CDirection<T>& direction) const
	{
		return *this - direction < 0;
	}
	bool operator<= (const CDirection<T>& direction) const
	{
		return *this - direction <= 0;
	}
	bool operator> (const CDirection<T>& direction) const
	{
		return *this - direction > 0;
	}
	bool operator>= (const CDirection<T>& direction) const
	{
		return *this - direction >= 0;
	}
};

#else

template <class T>
class CDirection : public CVector2<T>
{
public:
	CDirection()
		: CVector2<T>() {}
	CDirection(T dir)
		: x(cos(dir)), y(sin(dir)) {}
	CDirection(T x, T y)
		: CVector2<T>(x, y) {}
	CDirection(const CDirection<T>& dir)
		: CVector2<T>(dir) {}
	CDirection(const CVector2<T>& v)
		: CVector2<T>(v) {}
	template <class VAL_T>
	CDirection(const CVector2<VAL_T>& pt1, const CVector2<VAL_T>& pt2)
		: CVector2<T>(pt2.x - pt1.x, pt2.y - pt1.y) {}
	CDirection(T x1, T y1, T x2, T y2)
		: CVector2<T>(x2 - x1, y2 - y1) {}

	template <class DES_T>
	operator CDirection<DES_T>() const
	{
		CDirection<DES_T> dest_p;
		dest_p.x = (DES_T)x;
		dest_p.y = (DES_T)y;
		return dest_p;
	}
	operator T() const
	{
		return atan(y / x);

		if (x == 0 && y == 0)
			return NAN;
		if ((CVector2)(*this) / CVector2(1, 0) > 0)
			return acos((CVector2)(*this) * CVector2(1, 0) / (T)(CVector2)(*this));
		else
			return -acos((CVector2)(*this) * CVector2(1, 0) / (T)(CVector2)(*this));
	}

	// 运算：
	CDirection<T>& operator= (T dir)
	{
		x = cos(dir);
		y = sin(dir);
		return *this;
	}

	bool operator== (const CDirection<T>& direction) const
	{
		return *this - direction == 0;
	}
	bool operator!= (const CDirection<T>& direction) const
	{
		return *this - direction != 0;
	}
	bool operator< (const CDirection<T>& direction) const
	{
		return *this - direction < 0;
	}
	bool operator<= (const CDirection<T>& direction) const
	{
		return *this - direction <= 0;
	}
	bool operator> (const CDirection<T>& direction) const
	{
		return *this - direction > 0;
	}
	bool operator>= (const CDirection<T>& direction) const
	{
		return *this - direction >= 0;
	}

	CDirection<T> operator- () const
	{
		return CDirection<T>(-x, -y);
	}

	CDirection<T> operator- (T dir) const
	{
		CDirection<T> r(*this);
		r.RotationSelf(-dir);
		return r;
	}
	CDirection<T>& operator-= (T dir)
	{
		RotationSelf(-dir);
		return *this;
	}
	T operator- (const CDirection<T>& direction) const
	{
		if ((CVector2)(*this) / (CVector2)direction > 0)
			return acos((CVector2)(*this) * (CVector2)direction / (T)(CVector2)(*this) / (T)(CVector2)direction);
		else
			return -acos((CVector2)(*this) * (CVector2)direction / (T)(CVector2)(*this) / (T)(CVector2)direction);
	}

	CDirection<T> operator+ (T dir) const
	{
		CDirection<T> r(*this);
		r.RotationSelf(dir);
		return r;
	}
	CDirection<T>& operator+= (T dir)
	{
		RotationSelf(dir);
		return *this;
	}
	CDirection<T> operator+ (const CDirection<T>& direction) const
	{
		CDirection<T> r = *this;
		r += direction;
		return r;
	}
	CDirection<T>& operator+= (const CDirection<T>& direction)
	{
		x += direction.x;
		y += direction.y;
		return *this;
	}
};

#endif // SIMPLE_DIR

