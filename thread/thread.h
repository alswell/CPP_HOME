#pragma once
#include "../env.h"
#include <atomic>
#include <pthread.h>
//#include <signal.h>

template <class T>
class CThreadImpl
{
	bool volatile m_bStopped;
protected:
	pthread_t m_tid;

public:
	CThreadImpl()
	{
		m_tid = 0;
		m_bStopped = false;
	}
	int Start()
	{
		m_bStopped = false;
		return pthread_create(&m_tid, 0, ThreadProc, static_cast<void*>(this));
	}
	int Stop()
	{
		int r = pthread_cancel(m_tid);
		m_tid = 0;
		return r;
	}
	static void* ThreadProc(void* pData)
	{
		T* pThis = static_cast<T*>(pData);
		return pThis->Run();
	}
	void* Run()
	{
		return nullptr;
	}
	void* WaitForThread()
	{
		void* p_ret = nullptr;
		pthread_join(m_tid, &p_ret);
		return p_ret;
	}
};

template <class T_CLS, class T_CTXT, class T_ARG, class T_RESULT>
class CTaskMgr
{
	friend class CWorker;
	typedef void(T_CLS::*FUN)(int, T_CTXT&, unsigned, T_ARG&, T_RESULT&);
	FUN m_fun;
	typedef void(T_CLS::*INIT_CTXT)(T_CTXT&);
	INIT_CTXT m_funInitCtxt;
	T_CLS& m_obj;
	unsigned m_nTotal;
	atomic<unsigned> m_nCount;
	T_ARG* m_pArgs;
	T_RESULT* m_pResults;

	class CWorker : public CThreadImpl<CWorker>
	{
		int m_nTID;
		CTaskMgr& m_TaskMgr;
	public:
		CWorker(int nTID, CTaskMgr& mgr) : m_nTID(nTID), m_TaskMgr(mgr) {}
		void* Run()
		{
			T_CTXT ctxt;
			if (m_TaskMgr.m_funInitCtxt)
				((m_TaskMgr.m_obj).*m_TaskMgr.m_funInitCtxt)(ctxt);
			while (true)
			{
				auto index = m_TaskMgr.m_nCount.fetch_add(1);
				if (index >= m_TaskMgr.m_nTotal)
					break;
				((m_TaskMgr.m_obj).*m_TaskMgr.m_fun)(m_nTID, ctxt, index, m_TaskMgr.m_pArgs[index], m_TaskMgr.m_pResults[index]);
			}
			return nullptr;
		}
	};

public:
	CTaskMgr(T_CLS* obj, FUN f, unsigned n, T_ARG* pArgs, T_RESULT* pResults)
		: m_obj(*obj), m_fun(f), m_funInitCtxt(nullptr), m_nTotal(n), m_pArgs(pArgs), m_pResults(pResults)
	{}
	void RegInitCtxt(INIT_CTXT f)
	{
		m_funInitCtxt = f;
	}
	void Go()
	{
		m_nCount = 0;
		//DWORD dwT = GetTickCount();
		//int nCore = OS::GetCPUCore() + 1;
		int nCore = 6;
		vector<CWorker*> vThread(nCore);
		for (int x = 0; x < nCore; ++x)
		{
			vThread[x] = new CWorker(x, *this);
			vThread[x]->Start();
		}
		for (unsigned u = 0; u < vThread.size(); ++u)
		{
			vThread[u]->WaitForThread();
			delete vThread[u];
		}
		//return GetTickCount() - dwT;
	}
};

