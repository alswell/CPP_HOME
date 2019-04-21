#pragma once
#include "assert.h"

template <class T>
class cycl
{
	class CNode;
	CNode* m_pNode;
	unsigned int m_uCount;
public:
	cycl(): m_pNode(0), m_uCount(0) {}
	~cycl() {
		clear(); 
	}
	class iterator
	{
		friend class cycl<T>;
	protected:
		CNode* m_pListNode;
		//CycleListValue<T>* GetP() { return m_pCycleListValue; }
	public:
		iterator() : m_pListNode(0) {}
		iterator(CNode* pListNode) : m_pListNode(pListNode) {}

		bool operator== (iterator it)
		{
			return m_pListNode == it.m_pListNode;
		}
		bool operator!= (iterator it)
		{
			return m_pListNode != it.m_pListNode;
		}
		T& operator* ()
		{
			return m_pListNode->m_Data;
		}
		const T& operator* () const
		{
			return m_pListNode->m_Data;
		}
		T* operator-> ()
		{
			return &(m_pListNode->m_Data);
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
				m_pListNode = m_pListNode->m_pNext;

			return *this;
		}
		iterator& operator++ ()
		{
			m_pListNode = m_pListNode->m_pNext;
			return *this;
		}
		iterator operator++ (int)
		{
			iterator _old_it = *this;
			m_pListNode = m_pListNode->m_pNext;
			return _old_it;
		}
		iterator& operator-- ()
		{
			m_pListNode = m_pListNode->m_pPrior;
			return *this;
		}
		iterator operator-- (int)
		{
			iterator _old_it = *this;
			m_pListNode = m_pListNode->m_pPrior;
			return _old_it;
		}

		iterator operator+ ()
		{
			return iterator(m_pListNode->m_pNext);
		}
		iterator operator- ()
		{
			return iterator(m_pListNode->m_pPrior);
		}

		operator bool()
		{
			return m_pListNode;
		}
	};
	class iterator_ex : public iterator
	{
		unsigned m_nListSize;
		unsigned m_nCount;
	public:
		iterator_ex(cycl<T>& pList) : iterator(pList.entry()), m_nListSize(pList.size()), m_nCount(0) {}
		iterator& operator++ ()
		{
			++m_nCount;
			iterator::m_pListNode = iterator::m_pListNode->m_pNext;
			return *this;
		}
		operator bool()
		{
			return m_nCount < m_nListSize;
		}
	};
//	class const_iterator_ex : public iterator_ex
//	{
//	public:
//		const_iterator_ex(cycl<T>& pList) : iterator_ex(pList) {}
//		const T& operator* ()
//		{
//			return iterator::m_pListNode->m_Data;
//		}
//	};

	cycl(const cycl<T>& cl): m_pNode(0), m_uCount(0)
	{
		if (cl.size() == 0)
			return;
		cycl<T>::iterator it = cl.entry();
		do
		{
			insert_next(*it);
			it++;
		} while (it != cl.entry());
	}
	cycl<T>& operator= (const cycl<T>& cl)
	{
		if (cl.size() == 0)
			return *this;
		clear();
		cycl<T>::iterator it = cl.entry();
		do
		{
			insert_next(*it);
			it++;
		} while (it != cl.entry());
		return *this;
	}

	T& current()
	{
		return m_pNode->m_Data;
	}
	iterator entry() const
	{
		return iterator(m_pNode);
	}
	iterator set_entry(iterator it, bool check = true)
	{
		if (check)
		{
			iterator_ex _it = *this;
			for (; _it; ++_it)
				if (_it == it)
					break;
			assert(_it);
		}
		CNode* old_node = m_pNode;
		m_pNode = it.m_pListNode;
		return iterator(old_node);
	}
	iterator set_entry(const T& t, bool check = true)
	{
		iterator it = find(t);
		if (check)
			assert(it);
		if (it)
		{
			CNode* old_node = m_pNode;
			m_pNode = it.m_pListNode;
			it.m_pListNode = old_node;
		}
		return it;
	}

	unsigned int size() const
	{
		return m_uCount;
	}

	bool empty() const
	{
		return m_uCount == 0 || m_pNode == 0;
	}

	iterator insert_next(const T& t)
	{
		m_uCount++;

		CNode* pNode = new CNode;
		CNode* pNext = pNode;
		CNode* pPrior = pNode;
		if (m_pNode)
		{
			pNext = m_pNode->m_pNext;
			pPrior = m_pNode;
		}
		m_pNode = pNode;

		m_pNode->m_Data = t;
		m_pNode->m_pNext = pNext;
		m_pNode->m_pPrior = pPrior;

		pNext->m_pPrior = pNode;
		pPrior->m_pNext = pNode;
		
		return iterator(m_pNode);
	}

	iterator insert_next(const T& t, iterator it)
	{
		if (!it)
			return insert_next(t);

		m_uCount++;

		CNode* pNode = new CNode;
		CNode* pNext = it.m_pListNode->m_pNext;
		CNode* pPrior = it.m_pListNode;

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

		CNode* pNode = new CNode;
		CNode* pNext = pNode;
		CNode* pPrior = pNode;
		if (m_pNode)
		{
			pNext = m_pNode;
			pPrior = m_pNode->m_pPrior;
		}
		m_pNode = pNode;

		m_pNode->m_Data = t;
		m_pNode->m_pNext = pNext;
		m_pNode->m_pPrior = pPrior;

		pNext->m_pPrior = pNode;
		pPrior->m_pNext = pNode;

		return iterator(m_pNode);
	}

	iterator insert_prior(const T& t, iterator it)
	{
		m_uCount++;

		CNode* pNode = new CNode;
		CNode* pNext = it.m_pListNode;
		CNode* pPrior = it.m_pListNode->m_pPrior;

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
		CNode* _this_pStake_prior = m_pNode->m_pPrior;
		CNode* _that_pStake_prior = cyclist.m_pNode->m_pPrior;
		_this_pStake_prior->m_pNext = cyclist.m_pNode;
		m_pNode->m_pPrior = _that_pStake_prior;
		_that_pStake_prior->m_pNext = m_pNode;
		cyclist.m_pNode->m_pPrior = _this_pStake_prior;
		m_uCount += cyclist.m_uCount;

		cyclist.m_pNode = 0;
		cyclist.m_uCount = 0;
		return true;
	}

	iterator merge_back(cycl<T>& cyclist)
	{
		merge(cyclist);
		return iterator(m_pNode);
	}

	iterator merge_front(cycl<T>& cyclist)
	{
		CNode* tempStake = cyclist.m_pNode;
		if (merge(cyclist))
			m_pNode = tempStake;
		return iterator(m_pNode);
	}

	iterator erase(iterator it)
	{
		CNode* pNext = it.m_pListNode->m_pNext;
		CNode* pPrior = it.m_pListNode->m_pPrior;
		delete it.m_pListNode;
		if (m_pNode == it.m_pListNode)
			m_pNode = pNext;
		m_uCount--;
		if (m_uCount > 0)
		{
			pNext->m_pPrior = pPrior;
			pPrior->m_pNext = pNext;
		}
		else
		{
			m_pNode = 0;
		}
		return iterator(pNext);
	}

	void clear()
	{
		if (m_pNode)
		{
			m_uCount = 0;
			CNode* pNext = m_pNode->m_pNext;
			while (pNext != m_pNode)
			{
				m_pNode->m_pNext = pNext->m_pNext;
				delete pNext;
				pNext = m_pNode->m_pNext;
			}
			delete m_pNode;
			m_pNode = 0;
		}
	}

	iterator find(const T& t)
	{
		for (iterator_ex it = *this; it; ++it)
			if (t == *it)
				return it;
		return iterator();
	}
	template<class T_KEY, class CMP>
	iterator find(const T_KEY& key, CMP cmp)
	{
		for (iterator_ex it = *this; it; ++it)
			if (cmp(key, *it))
				return it;
		return iterator();
	}
private:
	class CNode
	{
		friend class cycl<T>;
		friend class cycl<T>::iterator;

		T m_Data;
		CNode* m_pNext;
		CNode* m_pPrior;
	public:
		CNode(): m_pNext(0), m_pPrior(0) {}
	};
};
