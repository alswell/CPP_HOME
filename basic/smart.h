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
	unsigned Count()
	{
		return m_nCount;
	}
	static CMemMgr* GetMgr(void* p)
	{
		if (p == nullptr)
			return nullptr; //New(1);
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
		, m_pArray(reinterpret_cast<T*>(CMemMgr<T>::New(len > cap ? len : cap)->Pointer()))
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
		auto n = CMemMgr<T>::GetMgr(m_pArray)->Count();
		if (m_nEnd > n)
			m_nEnd = n;
		CMemMgr<T>::GetMgr(m_pArray)->AddRef();
	}
	~CSlice()
	{
		if (m_pArray)
			CMemMgr<T>::GetMgr(m_pArray)->Release();
	}
	unsigned Len() const
	{
		return m_nEnd - m_nBeg;
	}
	unsigned Cap()
	{
		return CMemMgr<T>::GetMgr(m_pArray)->Count() - m_nBeg;
	}
	void Copy(const CSlice& slice)
	{
		auto len1 = Len();
		auto len2 = slice.Len();
		memcpy(&m_pArray[m_nBeg], &slice.m_pArray[slice.m_nBeg], sizeof(T) * (len1 < len2 ? len1 : len2));
	}
	CSlice& operator=(const CSlice& slice)
	{
		m_nBeg = slice.m_nBeg;
		m_nEnd = slice.m_nEnd;
		if (slice.m_pArray == m_pArray)
			return *this;
		if (m_pArray)
			CMemMgr<T>::GetMgr(m_pArray)->Release();
		m_pArray = slice.m_pArray;
		if (m_pArray)
			CMemMgr<T>::GetMgr(m_pArray)->AddRef();
		return *this;
	}
	T& operator[](int i)
	{
		if (i < 0)
			i += Len();
		return (m_pArray+m_nBeg)[i];
	}
	CSlice& operator+=(const CSlice& slice)
	{
		if (m_nEnd + slice.Len() > CMemMgr<T>::GetMgr(m_pArray)->Count())
		{
			CSlice s(Len(), Len() + slice.Len());
			s.Copy(*this);
			*this = s;
		}
		memcpy(&m_pArray[m_nEnd], &slice.m_pArray[slice.m_nBeg], sizeof(T) * slice.Len());
		m_nEnd += slice.Len();
		return *this;
	}
	CSlice& operator+(const CSlice& slice)
	{
		CSlice s(*this);
		s += slice;
		return s;
	}
	CSlice& operator+=(const T& x)
	{
		if (m_nEnd + 1 > CMemMgr<T>::GetMgr(m_pArray)->Count())
		{
			CSlice s(Len(), Len() + 1);
			s.Copy(*this);
			*this = s;
		}
		memcpy(&m_pArray[m_nEnd], &x, sizeof(T));
		m_nEnd += 1;
		return *this;
	}
	CSlice& operator+(const T& x)
	{
		CSlice s(*this);
		s += x;
		return s;
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
		if (obj.m_pObj == m_pObj)
			return m_pObj;
		if (m_pObj)
			CMemMgr<T>::GetMgr(m_pObj)->Release();
		m_pObj = obj.m_pObj;
		if (m_pObj)
			CMemMgr<T>::GetMgr(m_pObj)->AddRef();
		return m_pObj;
	}
	T* operator->() { return m_pObj; }
};
