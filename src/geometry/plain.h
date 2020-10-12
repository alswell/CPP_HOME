#pragma once
#include "obj3.h"

template <class T>
class CPlain : public C3DObj<T>
{
	void ResetABCD()
	{
		m_A = m_pt[0].y*(m_pt[1].z - m_pt[2].z) + m_pt[1].y*(m_pt[2].z - m_pt[0].z) + m_pt[2].y*(m_pt[0].z - m_pt[1].z);
		m_B = m_pt[0].z*(m_pt[1].x - m_pt[2].x) + m_pt[1].z*(m_pt[2].x - m_pt[0].x) + m_pt[2].z*(m_pt[0].x - m_pt[1].x);
		m_C = m_pt[0].x*(m_pt[1].y - m_pt[2].y) + m_pt[1].x*(m_pt[2].y - m_pt[0].y) + m_pt[2].x*(m_pt[0].y - m_pt[1].y);
		m_D = -m_pt[0].x*(m_pt[1].y*m_pt[2].z - m_pt[2].y*m_pt[1].z) - m_pt[1].x*(m_pt[2].y*m_pt[0].z - m_pt[0].y*m_pt[2].z) - m_pt[2].x*(m_pt[0].y*m_pt[1].z - m_pt[1].y*m_pt[0].z);
	}
public:
	CVector3<T> m_pt[3];
	T m_A, m_B, m_C, m_D;
	CPlain(const CVector3<T>& pt1, const CVector3<T>& pt2, const CVector3<T>& pt3)
	{
		m_pt[0] = pt1;
		m_pt[1] = pt2;
		m_pt[2] = pt3;
		for (int i = 0; i < 3; ++i)
			C3DObj<T>::m_lsPts.push_back(&m_pt[i]);
		ResetABCD();
	}

	void Move(const CVector3<T>& delta)
	{
		C3DObj<T>::Move(delta);
		ResetABCD();
	}
	void Rotation3DX(CVector3<T> pt0, T delta)
	{
		C3DObj<T>::Rotation3DX(pt0, delta);
		ResetABCD();
	}

	void Rotation3DY(CVector3<T> pt0, T delta)
	{
		C3DObj<T>::Rotation3DY(pt0, delta);
		ResetABCD();
	}

	void Rotation3DZ(CVector3<T> pt0, T delta)
	{
		C3DObj<T>::Rotation3DZ(pt0, delta);
		ResetABCD();
	}

	T Z(T x, T y)
	{
		return -(m_A * x + m_B * y + m_D) / m_C;
	}

	T Distance(CVector3<T> pt)
	{
		return abs(m_A * pt.x + m_B * pt.y + m_C * pt.z + m_D) / sqrt(SQUARE(m_A) + SQUARE(m_B) + SQUARE(m_C));
	}
};

template <class T>
class CPointNormalPlain : public C3DObj<T>
{
public:
	CVector3<T> m_M0;	// m0 @ plain
	CVector3<T> m_M1;
	CPointNormalPlain(CVector3<T> m0, CVector3<T> m1) : m_M0(m0), m_M1(m1)
	{
		C3DObj<T>::m_lsPts.push_back(&m_M0);
		C3DObj<T>::m_lsPts.push_back(&m_M1);
	}

	CVector3<T> NoramlVector() const
	{
		return CVector3<T>(m_M1.x - m_M0.x, m_M1.y - m_M0.y, m_M1.z - m_M0.z);
	}

	void Rotation3DX_M0(T delta) { C3DObj<T>::Rotation3DX(m_M0, delta); }
	void Rotation3DX_M1(T delta) { C3DObj<T>::Rotation3DX(m_M1, delta); }
	void Rotation3DY_M0(T delta) { C3DObj<T>::Rotation3DY(m_M0, delta); }
	void Rotation3DY_M1(T delta) { C3DObj<T>::Rotation3DY(m_M1, delta); }
	void Rotation3DZ_M0(T delta) { C3DObj<T>::Rotation3DZ(m_M0, delta); }
	void Rotation3DZ_M1(T delta) { C3DObj<T>::Rotation3DZ(m_M1, delta); }
};
