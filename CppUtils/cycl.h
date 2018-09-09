#pragma once


template <class T>
class cycl;

template <class T>
class CycleListValue
{
	friend class cycl<T>;
	friend class cycl<T>::iterator;

	T m_Data;
	CycleListValue* m_pNext;
	CycleListValue* m_pPrior;
public:
	CycleListValue(): m_pNext(0), m_pPrior(0) {}
};

template <class T>
class cycl
{
	CycleListValue<T>* m_pStake;
	unsigned int m_uCount;
public:
	cycl(): m_pStake(0), m_uCount(0) {}
	~cycl() {
		clear(); 
	}
	class iterator
	{
		friend class cycl<T>;

		CycleListValue<T>* m_pCycleListValue;
		//CycleListValue<T>* GetP() { return m_pCycleListValue; }
	public:
		iterator() : m_pCycleListValue(0) {}
		iterator(CycleListValue<T>* pCycleListValue) : m_pCycleListValue(pCycleListValue) {}

		bool operator== (iterator it)
		{
			return m_pCycleListValue == it.m_pCycleListValue;
		}
		bool operator!= (iterator it)
		{
			return m_pCycleListValue != it.m_pCycleListValue;
		}
		T& operator* ()
		{
			return m_pCycleListValue->m_Data;
		}
		const T& operator* () const
		{
			return m_pCycleListValue->m_Data;
		}
		T* operator-> ()
		{
			return &(m_pCycleListValue->m_Data);
		}
		iterator operator+ (int n)
		{
			iterator _new_it = *this;
			if (n < 0)
				n = 0;
			for (int i = 0; i < n; i++)
				_new_it = _new_it->m_pNext;

			return _new_it;
		}
		iterator& operator+= (int n)
		{
			if (n < 0)
				n = 0;
			for (int i = 0; i < n; i++)
				m_pCycleListValue = m_pCycleListValue->m_pNext;

			return *this;
		}
		iterator& operator++ ()
		{
			m_pCycleListValue = m_pCycleListValue->m_pNext;
			return *this;
		}
		iterator operator++ (int)
		{
			iterator _old_it = *this;
			m_pCycleListValue = m_pCycleListValue->m_pNext;
			return _old_it;
		}
		iterator& operator-- ()
		{
			m_pCycleListValue = m_pCycleListValue->m_pPrior;
			return *this;
		}
		iterator operator-- (int)
		{
			iterator _old_it = *this;
			m_pCycleListValue = m_pCycleListValue->m_pPrior;
			return _old_it;
		}

		iterator operator+ ()
		{
			return iterator(m_pCycleListValue->m_pNext);
		}
		iterator operator- ()
		{
			return iterator(m_pCycleListValue->m_pPrior);
		}

		operator bool()
		{
			return m_pCycleListValue;
		}

	};
	cycl(const cycl<T>& cl): m_pStake(0), m_uCount(0)
	{
		if (cl.size() == 0)
			return;
		cycl<T>::iterator it = cl.begin();
		do 
		{
			insert_next(*it);
			it++;
		} while (it != cl.begin());
	}
	cycl<T>& operator= (const cycl<T>& cl)
	{
		if (cl.size() == 0)
			return *this;
		clear();
		cycl<T>::iterator it = cl.begin();
		do 
		{
			insert_next(*it);
			it++;
		} while (it != cl.begin());
		return *this;
	}

	iterator begin() const
	{
		return iterator(m_pStake);
	}
	iterator set_begin(iterator it)
	{
		m_pStake = it.m_pCycleListValue;
		return it;
	}

	unsigned int size() const
	{
		return m_uCount;
	}

	bool empty() const
	{
		return m_uCount == 0 || m_pStake == 0;
	}

	iterator insert_next(const T& t)
	{
		m_uCount++;

		CycleListValue<T>* pNode = new CycleListValue<T>;
		CycleListValue<T>* pNext = pNode;
		CycleListValue<T>* pPrior = pNode;
		if (m_pStake)
		{
			pNext = m_pStake->m_pNext;
			pPrior = m_pStake;
		}
		m_pStake = pNode;

		m_pStake->m_Data = t;
		m_pStake->m_pNext = pNext;
		m_pStake->m_pPrior = pPrior;

		pNext->m_pPrior = pNode;
		pPrior->m_pNext = pNode;
		
		return iterator(m_pStake);
	}

	iterator insert_next(const T& t, iterator it)
	{
		if (!it)
			return insert_next(t);

		m_uCount++;

		CycleListValue<T>* pNode = new CycleListValue<T>;
		CycleListValue<T>* pNext = it.m_pCycleListValue->m_pNext;
		CycleListValue<T>* pPrior = it.m_pCycleListValue;

		pNode->m_Data = t;
		pNode->m_pNext = pNext;
		pNode->m_pPrior = pPrior;

		pNext->m_pPrior = pNode;
		pPrior->m_pNext = pNode;

		return iterator(pNode);
	}

	iterator insert_prior(const T& t)
	{
		m_uCount++;

		CycleListValue<T>* pNode = new CycleListValue<T>;
		CycleListValue<T>* pNext = pNode;
		CycleListValue<T>* pPrior = pNode;
		if (m_pStake)
		{
			pNext = m_pStake;
			pPrior = m_pStake->m_pPrior;
		}
		m_pStake = pNode;

		m_pStake->m_Data = t;
		m_pStake->m_pNext = pNext;
		m_pStake->m_pPrior = pPrior;

		pNext->m_pPrior = pNode;
		pPrior->m_pNext = pNode;

		return iterator(m_pStake);
	}

	iterator insert_prior(const T& t, iterator it)
	{
		m_uCount++;

		CycleListValue<T>* pNode = new CycleListValue<T>;
		CycleListValue<T>* pNext = it.m_pCycleListValue;
		CycleListValue<T>* pPrior = it.m_pCycleListValue->m_pPrior;

		pNode->m_Data = t;
		pNode->m_pNext = pNext;
		pNode->m_pPrior = pPrior;

		pNext->m_pPrior = pNode;
		pPrior->m_pNext = pNode;

		return iterator(pNode);
	}

	bool merge(cycl<T>& cyclist)
	{
		if (cyclist.empty())
			return false;
		CycleListValue<T>* _this_pStake_prior = m_pStake->m_pPrior;
		CycleListValue<T>* _that_pStake_prior = cyclist.m_pStake->m_pPrior;
		_this_pStake_prior->m_pNext = cyclist.m_pStake;
		m_pStake->m_pPrior = _that_pStake_prior;
		_that_pStake_prior->m_pNext = m_pStake;
		cyclist.m_pStake->m_pPrior = _this_pStake_prior;
		m_uCount += cyclist.m_uCount;

		cyclist.m_pStake = 0;
		cyclist.m_uCount = 0;
		return true;
	}

	iterator merge_back(cycl<T>& cyclist)
	{
		merge(cyclist);
		return iterator(m_pStake);
	}

	iterator merge_front(cycl<T>& cyclist)
	{
		CycleListValue<T>* tempStake = cyclist.m_pStake;
		if (merge(cyclist))
			m_pStake = tempStake;
		return iterator(m_pStake);
	}

	iterator erase(iterator it)
	{
		CycleListValue<T>* pNext = it.m_pCycleListValue->m_pNext;
		CycleListValue<T>* pPrior = it.m_pCycleListValue->m_pPrior;
		delete it.m_pCycleListValue;
		if (m_pStake == it.m_pCycleListValue)
			m_pStake = pNext;
		m_uCount--;
		if (m_uCount > 0)
		{
			pNext->m_pPrior = pPrior;
			pPrior->m_pNext = pNext;
		}
		else
		{
			m_pStake = 0;
		}
		return iterator(pNext);
	}

	void clear()
	{
		if (m_pStake)
		{
			m_uCount = 0;
			CycleListValue<T>* pNext = m_pStake->m_pNext;
			while (pNext != m_pStake)
			{
				m_pStake->m_pNext = pNext->m_pNext;
				delete pNext;
				pNext = m_pStake->m_pNext;
			}
			delete m_pStake;
			m_pStake = 0;
		}
	}
};
