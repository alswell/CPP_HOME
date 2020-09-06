#pragma once
#include "env.h"
#include "cycl.h"

#define ITER(p, implIter) for (auto p = (implIter).Start(); implIter; p = ++(implIter))

template <class T>
class IIterator
{
public:
	virtual ~IIterator() {}
	virtual unsigned Size() = 0;
	virtual T Start() = 0;
	virtual T operator++() = 0;
	virtual operator bool() = 0;
};

template <class T_ELEM, class T_RETURN>
class CVectorIter : public IIterator<T_RETURN*>
{
protected:
	vector<T_ELEM>& m_ls;
	typename::vector<T_ELEM>::iterator m_it;
public:
	CVectorIter(vector<T_ELEM>& ls) : m_ls(ls) {}
	virtual unsigned Size()
	{
		return m_ls.size();
	}
	virtual T_RETURN* Start()
	{
		m_it = m_ls.begin();
		return GetIterItem(*m_it);
	}
	virtual T_RETURN* operator++()
	{
		++m_it;
		return GetIterItem(*m_it);
	}
	virtual operator bool()
	{
		return m_it != m_ls.end();
	}
};

template <class T_ELEM, class T_RETURN>
class CListIter : public IIterator<T_RETURN*>
{
protected:
	list<T_ELEM>& m_ls;
	typename::list<T_ELEM>::iterator m_it;
public:
	CListIter(list<T_ELEM>& ls) : m_ls(ls) {}
	virtual unsigned Size()
	{
		return m_ls.size();
	}
	virtual T_RETURN* Start()
	{
		m_it = m_ls.begin();
		return GetIterItem(*m_it);
	}
	virtual T_RETURN* operator++()
	{
		++m_it;
		return GetIterItem(*m_it);
	}
	virtual operator bool()
	{
		return m_it != m_ls.end();
	}
};

template <class T_ELEM, class T_RETURN>
class CCyclIter : public IIterator<T_RETURN*>
{
protected:
	cycl<T_ELEM>& m_ls;
	typename::cycl<T_ELEM>::iterator m_it;
	unsigned m_nCount;
public:
	CCyclIter(cycl<T_ELEM>& ls) : m_ls(ls), m_nCount(0) {}
	virtual unsigned Size()
	{
		return m_ls.size();
	}
	virtual T_RETURN* Start()
	{
		m_nCount = 0;
		m_it = m_ls.entry();
		return GetIterItem(*m_it);
	}
	virtual T_RETURN* operator++()
	{
		++m_nCount;
		++m_it;
		return GetIterItem(*m_it);
	}
	virtual operator bool()
	{
		return m_nCount < m_ls.size();
	}
};

template <class T_ELEM, class T_RETURN>
class CCyclSegIter : public IIterator<T_RETURN*>
{
	typename::cycl<T_ELEM>::iterator m_it0, m_it1, m_it;
	bool m_bStop;
public:
	CCyclSegIter(typename::cycl<T_ELEM>::iterator it0, typename::cycl<T_ELEM>::iterator it1) : m_it0(it0), m_it1(it1), m_it(it0){}
	virtual unsigned Size()
	{
		unsigned count = 1;
		for (auto it = m_it0; it != m_it1; ++it, ++count);
		return count;
	}
	virtual T_RETURN* Start()
	{
		m_bStop = false;
		m_it = m_it0;
		return GetIterItem(*m_it);
	}
	virtual T_RETURN* operator++()
	{
		if (m_it == m_it1)
		{
			m_bStop = true;
			return nullptr;
		}
		++m_it;
		return GetIterItem(*m_it);
	}
	virtual operator bool()
	{
		return !m_bStop;
	}
};

