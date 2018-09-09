#pragma once
#include <vector>
#include <list>

template <class T>
class arr
{
	T* m_pT;
	int m_nSize;
	int m_nBuffSize;
public:

	arr(int nSize = 0)
	{
		if (nSize > 0)
		{
			m_nBuffSize = nSize;
			m_nSize = nSize;
			m_pT = new T[m_nBuffSize];
			memset(m_pT, 0, m_nBuffSize * sizeof(T));
		}
		else
		{
			m_pT = NULL;
			m_nSize = 0;
			m_nBuffSize = 0;
		}
	}
	arr(const arr<T>& v)
		: arr(0)
	{
		(*this) = v;
	}
	arr(const std::vector<T>& v)
	{
		m_nSize = v.size();
		m_nBuffSize = m_nSize;
		m_pT = new T[m_nBuffSize];
		memset(m_pT, 0, m_nBuffSize * sizeof(T));
		for (int i = 0; i < m_nSize; ++i)
			m_pT[i] = v[i];
	}
	arr(const std::list<T>& v)
	{
		m_nSize = v.size();
		m_nBuffSize = m_nSize;
		m_pT = new T[m_nBuffSize];
		memset(m_pT, 0, m_nBuffSize * sizeof(T));
		typename std::list<T>::const_iterator it = v.begin();
		for (int i = 0; i < m_nSize; ++i, ++it)
			m_pT[i] = *it;
	}
	~arr()
	{
		if (m_pT)
		{
			delete[] m_pT;
			m_pT = NULL;
			m_nSize = 0;
			m_nBuffSize = 0;
		}
	}

	const T* data() const
	{
		return m_pT;
	}
	unsigned size() const
	{
		return m_nSize;
	}

	void operator = (const std::vector<T>& v)
	{
		if (m_nBuffSize < v.size())
		{
			if (m_pT)
				delete[] m_pT;
			m_nBuffSize = v.size();
			m_pT = new T[m_nBuffSize];
			memset(m_pT, 0, m_nBuffSize * sizeof(T));
		}

		m_nSize = v.size();
		for (int i = 0; i < m_nSize; ++i)
			m_pT[i] = v[i];
	}
	void operator = (const std::list<T>& v)
	{
		if (m_nBuffSize < v.size())
		{
			if (m_pT)
				delete[] m_pT;
			m_nBuffSize = v.size();
			m_pT = new T[m_nBuffSize];
			memset(m_pT, 0, m_nBuffSize * sizeof(T));
		}

		m_nSize = v.size();
		typename std::list<T>::const_iterator it = v.begin();
		for (int i = 0; i < m_nSize; ++i, ++it)
			m_pT[i] = *it;
	}

	void operator = (const arr<T>& v)
	{
		resize(v.m_nBuffSize);
		(*this) = v.m_pT;
	}
	void operator = (const T* p)
	{
		memcpy(m_pT, p, m_nBuffSize * sizeof(T));
		m_nSize = m_nBuffSize;
	}

	operator T* ()
	{
		return m_pT;
	}

	T& operator [] (int n)
	{
		return m_pT[n];
	}

	void resize(int nSize)
	{
		if (m_nBuffSize >= nSize)
		{
			m_nSize = nSize;
			return;
		}

		T* pOld = m_pT;
		m_pT = new T[nSize];
		memset(m_pT, 0, nSize * sizeof(T));
		if (pOld)
		{
			memcpy(m_pT, pOld, m_nSize * sizeof(T));
			delete[] pOld;
		}
		m_nBuffSize = nSize;
	}

//	void copy(arr<T>& v)
//	{
//		v.resize(m_nBuffSize);
//		v = m_pT;
//	}
	void to_v(std::vector<T>& v) const
	{
		v.resize(m_nSize);
		for (int i = 0; i < m_nSize; ++i)
			v[i] = m_pT[i];
	}
};
