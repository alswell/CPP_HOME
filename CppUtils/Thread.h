#ifndef THREAD_H
#define THREAD_H
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
	virtual int Start()
	{
		m_bStopped = false;
		return pthread_create(&m_tid, 0, ThreadProc, (void*)this);
	}
	virtual int Stop()
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
		return 0;
	}
	void* WaitForThread()
	{
		void* p_ret = 0;
		pthread_join(m_tid, &p_ret);
		return p_ret;
	}
};

#endif
