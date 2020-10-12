#pragma once
#include "env.h"
#include "vector3.h"


template <class T>
class C3DObj
{
	//protected:
public:
	list<CVector3<T>*> m_lsPts;
	//void AddPoint(const CPoint3D<T>& pt)
	//{
	//	m_lsPts.push_back(new CPoint3D<T>(pt));
	//}

	void Move(const CVector3<T>& delta)
	{
		for (auto it = m_lsPts.begin(); it != m_lsPts.end(); ++it)
			(**it) += delta;
	}

	void Rotation3DX(const CVector3<T>& pt0, T delta)
	{
		for (auto it = m_lsPts.begin(); it != m_lsPts.end(); ++it)
			**it = (**it).Rotation3DX(pt0, delta);
	}
	void Rotation3DY(const CVector3<T>& pt0, T delta)
	{
		for (auto it = m_lsPts.begin(); it != m_lsPts.end(); ++it)
			**it = (**it).Rotation3DY(pt0, delta);
	}
	void Rotation3DZ(const CVector3<T>& pt0, T delta)
	{
		for (auto it = m_lsPts.begin(); it != m_lsPts.end(); ++it)
			**it = (**it).Rotation3DZ(pt0, delta);
	}

	void ZoomIn(float n)
	{
		for (auto it = m_lsPts.begin(); it != m_lsPts.end(); ++it)
			(**it) *= n;
	}
	void ZoomOut(float n)
	{
		for (auto it = m_lsPts.begin(); it != m_lsPts.end(); ++it)
			(**it) /= n;
	}
};
