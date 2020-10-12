#pragma once

#define DEBUG_CYCQ

template <class T>
class cycq
{
public:
	T* m_pQ;
	int m_qf, m_qr;
	unsigned m_nSize;
	unsigned m_nMax;

	cycq(unsigned nSize = 4096): m_nSize(nSize), m_qf(0), m_qr(0), m_nMax(0)
	{
		m_pQ = new T[m_nSize];
	}
	~cycq()
	{
		delete[] m_pQ;
	}
	inline bool empty()
	{
		return m_qf == m_qr;
	}

	inline bool full()
	{
		return ((m_qr + 1) % m_nSize) == m_qf;
	}

	inline unsigned size()
	{
		return (m_qr + m_nSize - m_qf) % m_nSize;
	}

	T& front()
	{
		return m_pQ[(m_qf + 1) % m_nSize];
	}
	T& back()
	{
		return m_pQ[m_qr];
	}

	bool push_back(const T& e)
	{
		assert(!full());
#ifdef DEBUG_CYCQ
		if (size() > m_nMax) m_nMax = size();
#endif
		if (full())
			return false;
		++m_qr; 
		m_qr %= m_nSize;
		m_pQ[m_qr] = e;
		return true;
	}

	bool pop_front(T& e)
	{
		if (empty())
			return false;
		++m_qf; 
		m_qf %= m_nSize;
		e = m_pQ[m_qf];
		return true;
	}
	T* pop_front()
	{
		if (empty())
			return NULL;
		++m_qf;
		m_qf %= m_nSize;
		return &m_pQ[m_qf];;
	}
};
