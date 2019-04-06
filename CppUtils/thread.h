#pragma once
#include <pthread.h>
//#include <signal.h>

template< class T >
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

