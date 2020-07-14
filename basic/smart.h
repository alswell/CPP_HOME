#pragma once
#include "env.h"

template <typename T>
class CMemMgr
{
	unsigned m_nRef;
	unsigned m_nCount;
public:
	T* Pointer()
	{
		return reinterpret_cast<T*>(this + 1);
	}
	static CMemMgr* GetMgr(void* p)
	{
		if (p == nullptr)
			return New(1);
		return reinterpret_cast<CMemMgr*>(p) - 1;
	}
	void AddRef()
	{
		++m_nRef;
	}
	void Release()
	{
		--m_nRef;
		if (m_nRef == 0)
		{
			auto p = Pointer();
			for (unsigned i = 0; i < m_nCount; ++i)
				(&p[i])->~T();
			free(this);
		}
	}
	static CMemMgr* New(unsigned n)
	{
		CMemMgr* p = reinterpret_cast<CMemMgr*>(malloc(sizeof(CMemMgr) + n * sizeof(T)));
		p->m_nRef = 1;
		p->m_nCount = n;
		new (p->Pointer()) T[n];
		return p;
	}
	static CMemMgr* New(const T& x, unsigned n)
	{
		CMemMgr* p = reinterpret_cast<CMemMgr*>(malloc(sizeof(CMemMgr) + n * sizeof(T)));
		p->m_nRef = 1;
		p->m_nCount = n;
		auto arr = p->Pointer();
		for (unsigned i = 0; i < n; ++i)
			new (&arr[i]) T(x);
		return p;
	}
};

template <typename T>
class CSlice
{
	unsigned m_nBeg;
	unsigned m_nEnd;
	T* m_pArray;
public:
	CSlice() : m_nBeg(0), m_nEnd(0), m_pArray(nullptr) {}
	CSlice(unsigned len, unsigned cap)
			: m_nBeg(0)
			, m_nEnd(len)
			, m_pArray(reinterpret_cast<T*>(CMemMgr<T>::New(cap)->Pointer()))
	{}
	CSlice(const CSlice& slice)
			: m_nBeg(slice.m_nBeg)
			, m_nEnd(slice.m_nEnd)
			, m_pArray(slice.m_pArray)
	{
		CMemMgr<T>::GetMgr(m_pArray)->AddRef();
	}
	CSlice(const CSlice& slice, unsigned beg, unsigned end)
			: m_nBeg(slice.m_nBeg + beg)
			, m_nEnd(slice.m_nBeg + end)
			, m_pArray(slice.m_pArray)
	{
		CMemMgr<T>::GetMgr(m_pArray)->AddRef();
	}
	~CSlice()
	{
		CMemMgr<T>::GetMgr(m_pArray)->Release();
	}
	unsigned Len()
	{
		return m_nEnd - m_nBeg;
	}
	T& operator[] (unsigned i)
	{
		return (m_pArray+m_nBeg)[i];
	}
	void Print()
	{
		Output(m_pArray + m_nBeg, Len());
	}
};

template <typename T>
void Output(CSlice<T> slice)
{
	slice.Print();
}

template <typename T>
class CSmartObj
{
	T* m_pObj;
public:
	CSmartObj() : m_pObj(nullptr) {}
	CSmartObj(const CSmartObj& obj)
			: m_pObj(obj.m_pObj)
	{
		CMemMgr<T>::GetMgr(m_pObj)->AddRef();
	}
	~CSmartObj()
	{
		CMemMgr<T>::GetMgr(m_pObj)->Release();
	}
	static CSmartObj New()
	{
		CSmartObj obj;
		obj.m_pObj = reinterpret_cast<T*>(CMemMgr<T>::New(1)->Pointer());
		return obj;
	}
	static CSmartObj New(const T& x)
	{
		CSmartObj obj;
		obj.m_pObj = reinterpret_cast<T*>(CMemMgr<T>::New(x, 1)->Pointer());
		return obj;
	}
	T* operator=(const CSmartObj& obj)
	{
		if (obj.m_pObj == nullptr)
			return nullptr;
		if (obj.m_pObj == m_pObj)
			return m_pObj;
		if (m_pObj)
			CMemMgr<T>::GetMgr(m_pObj)->Release();
		m_pObj = obj.m_pObj;
		CMemMgr<T>::GetMgr(m_pObj)->AddRef();
		return m_pObj;
	}
	T* operator->() { return m_pObj; }
};
