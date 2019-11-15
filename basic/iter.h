#pragma once
#include "vector2.h"
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
class IListIter : public IIterator<T_RETURN*>
{
protected:
	list<T_ELEM>& m_ls;
	typename::list<T_ELEM>::iterator m_it;
public:
	IListIter(list<T_ELEM>& ls) : m_ls(ls) {}
	virtual T_RETURN* Get() = 0;
	virtual unsigned Size()
	{
		return m_ls.size();
	}
	virtual T_RETURN* Start()
	{
		m_it = m_ls.begin();
		return Get();
	}
	virtual T_RETURN* operator++()
	{
		++m_it;
		return Get();
	}
	virtual operator bool()
	{
		return m_it != m_ls.end();
	}
};

template <class T_ELEM, class T_RETURN>
class ICyclIter : public IIterator<T_RETURN*>
{
protected:
	cycl<T_ELEM>& m_ls;
	typename::cycl<T_ELEM>::iterator_ex m_it;
public:
	ICyclIter(cycl<T_ELEM>& ls) : m_ls(ls), m_it(ls) {}
	virtual T_RETURN* Get() = 0;
	virtual unsigned Size()
	{
		return m_ls.size();
	}
	virtual T_RETURN* Start()
	{
		m_it = m_ls.entry_ex();
		return Get();
	}
	virtual T_RETURN* operator++()
	{
		++m_it;
		return Get();
	}
	virtual operator bool()
	{
		return m_it;
	}
};
