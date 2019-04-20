#pragma once
#include "direction.h"

template <class T>
class CVector3
{
public:
	T x;
	T y;
	T z;
	__device__ CVector3() : x(0), y(0), z(0) {}
	__device__ CVector3(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {}
	__device__ CVector3(const CVector2<T>& pt) : x(pt.x), y(pt.y), z(0) {}

	void operator = (const CVector2<T>& pt)
	{
		x = pt.x;
		y = pt.y;
	}

	bool operator == (CVector3<T> pt)
	{
		return x == pt.x && y == pt.y && z == pt.z;
	}

	bool operator != (CVector3<T> pt)
	{
		return !(*this == pt);
	}

	template <class DES_T>
	operator CVector3<DES_T>() const
	{
		CVector3<DES_T> dest_p;
		dest_p.x = (DES_T)x;
		dest_p.y = (DES_T)y;
		dest_p.z = (DES_T)z;
		return dest_p;
	}

	template <class DES_T>
	operator CVector2<DES_T>() const
	{
		CVector2<DES_T> dest_p;
		dest_p.x = (DES_T)x;
		dest_p.y = (DES_T)y;
		return dest_p;
	}

	CVector3<T> operator- () const
	{
		return CVector3<T>(-x, -y, -z);
	}

	CVector3<T>& operator += (CVector3<T> v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	CVector3<T> operator + (CVector3<T> v) const
	{
		CVector3<T> r = *this;
		r += v;
		return r;
	}
	CVector3<T>& operator -= (CVector3<T> v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	CVector3<T> operator - (CVector3<T> v) const
	{
		CVector3<T> r = *this;
		r -= v;
		return r;
	}

	CVector3<T>& operator += (T n)
	{
		x += n;
		y += n;
		z += n;
		return *this;
	}
	CVector3<T> operator + (T n) const
	{
		CVector3<T> r = *this;
		r += n;
		return r;
	}
	CVector3<T>& operator -= (T n)
	{
		x -= n;
		y -= n;
		z -= n;
		return *this;
	}
	CVector3<T> operator - (T n) const
	{
		CVector3<T> r = *this;
		r -= n;
		return r;
	}
	CVector3<T>& operator *= (T n)
	{
		x *= n;
		y *= n;
		z *= n;
		return *this;
	}
	CVector3<T> operator * (T n) const
	{
		CVector3<T> r = *this;
		r *= n;
		return r;
	}
	CVector3<T>& operator /= (T n)
	{
		x /= n;
		y /= n;
		z /= n;
		return *this;
	}
	CVector3<T> operator / (T n) const
	{
		CVector3<T> r = *this;
		r /= n;
		return r;
	}

	__device__ void Rotation3DXSelf(T delta)
	{
		CVector2<T> pt_yz(y, z);
		pt_yz.RotationSelf(delta);
		y = pt_yz.x;
		z = pt_yz.y;
	}
	void Rotation3DXSelf(const CVector3<T>& pt0, T delta)
	{
		(*this) -= pt0;
		this->Rotation3DXSelf(delta);
		(*this) += pt0;
	}
	CVector3<T> Rotation3DX(T delta) const
	{
		CVector2<T> pt_yz(y, z);
		pt_yz.RotationSelf(delta);
		return CVector3<T>(x, pt_yz.x, pt_yz.y);
	}
	CVector3<T> Rotation3DX(const CVector3<T>& pt0, T delta) const
	{
		CVector3<T> pt = *this;
		pt -= pt0;
		pt.Rotation3DXSelf(delta);
		pt += pt0;
		return pt;
	}

	__device__ void Rotation3DYSelf(T delta)
	{
		CVector2<T> pt_xz(x, z);
		pt_xz.RotationSelf(delta);
		x = pt_xz.x;
		z = pt_xz.y;
	}
	void Rotation3DYSelf(const CVector3<T>& pt0, T delta)
	{
		(*this) -= pt0;
		this->Rotation3DYSelf(delta);
		(*this) += pt0;
	}
	CVector3<T> Rotation3DY(T delta) const
	{
		CVector2<T> pt_xz(x, z);
		pt_xz.RotationSelf(delta);
		return CVector3<T>(pt_xz.x, y, pt_xz.y);
	}
	CVector3<T> Rotation3DY(const CVector3<T>& pt0, T delta) const
	{
		CVector3<T> pt = *this;
		pt -= pt0;
		pt.Rotation3DYSelf(delta);
		pt += pt0;
		return pt;
	}

	__device__ void Rotation3DZSelf(T delta)
	{
		CVector2<T> pt_xy(x, y);
		pt_xy.RotationSelf(delta);
		x = pt_xy.x;
		y = pt_xy.y;
	}
	void Rotation3DZSelf(const CVector3<T>& pt0, T delta)
	{
		(*this) -= pt0;
		this->Rotation3DZSelf(delta);
		(*this) += pt0;
	}
	CVector3<T> Rotation3DZ(T delta) const
	{
		CVector2<T> pt_xy(x, y);
		pt_xy.RotationSelf(delta);
		return CVector3<T>(pt_xy.x, pt_xy.y, z);
	}
	CVector3<T> Rotation3DZ(const CVector3<T>& pt0, T delta) const
	{
		CVector3<T> pt = *this;
		pt -= pt0;
		pt.Rotation3DZSelf(delta);
		pt += pt0;
		return pt;
	}
	void NormalizeSelf()
	{
		T length = sqrt(SQUARE(x) + SQUARE(y) + SQUARE(z));
		x /= length;
		y /= length;
		z /= length;
	}
	void Rotation3DSelf(CVector3<T> v, T delta)
	{
		T p[3];
		T q[3];
		T r[3];

		v.NormalizeSelf();
		T sin_delta = sinf(delta);
		T cos_delta = cosf(delta);
		T _cos = 1 - cos_delta;
		p[0] = SQUARE(v.x) * _cos + cos_delta;
		q[1] = SQUARE(v.y) * _cos + cos_delta;
		r[2] = SQUARE(v.z) * _cos + cos_delta;

		T xy_cos = v.x * v.y * _cos;
		T z_sin = v.z * sin_delta;
		T xz_cos = v.x * v.z * _cos;
		T y_sin = v.y * sin_delta;
		T yz_cos = v.y * v.z * _cos;
		T x_sin = v.x * sin_delta;

		p[1] = xy_cos - z_sin;
		p[2] = xz_cos + y_sin;
		q[0] = xy_cos + z_sin;
		q[2] = yz_cos - x_sin;
		r[0] = xz_cos - y_sin;
		r[1] = yz_cos + x_sin;

		T tmp_x = p[0] * x + p[1] * y + p[2] * z;
		T tmp_y = q[0] * x + q[1] * y + q[2] * z;
		T tmp_z = r[0] * x + r[1] * y + r[2] * z;
		x = tmp_x;
		y = tmp_y;
		z = tmp_z;
	}

	T Dot(const CVector3<T>& v2)
	{
		return x * v2.x + y * v2.y + z * v2.z;
	}
	CVector3<T> Cross(const CVector3<T>& v2)
	{
		return CVector3<T>(y*v2.z - v2.y*z, v2.x*z - x*v2.z, x*v2.y - v2.x*y);
	}

	T DirectionX()
	{
		return CDirection<float>(y, z);
	}
	T DirectionY()
	{
		return CDirection<float>(x, z);
	}
	T DirectionZ()
	{
		return CDirection<float>(x, y);
	}

	T Longitude()
	{
		return CDirection<float>(x, y);
	}

	T Latitude()
	{
		CVector3<T> v = Rotation3DZ(-Longitude());
		return CDirection<float>(v.x, v.z);
	}
};

template <class T>
const char* ToStr(CVector3<T> v)
{
	return SPRINTF("[%s, %s, %s]", _(v.x), _(v.y), _(v.z));
}

