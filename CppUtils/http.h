#ifndef __HTTP_H
#define __HTTP_H
#include "thread.h"
#include "sock.h"
#include "json.h"


class HttpResponse
{
	CSock m_sock;
	CString m_strResp;
	CString m_strVersion;
	int m_nStatus;
	CString m_strStatus;
	CString m_strBody;
	DICT(CString) m_dHead;
public:
	HttpResponse(int status = 200, float version = 1.1);
	HttpResponse(CSock _sock);

	CString &operator [] (const CString& key);
	HttpResponse &operator () (const CString& strBody);
	CString &operator () ();
};

class HttpRequest
{
	CSock m_sock;
	CString m_strReq;
	CString m_strMethod;
	CString m_strUrl;
	CString m_strBody;
	float m_fVersion;
	DICT(CString) m_dHead;
public:
	HttpRequest(const char* host, int port = 80, const char* url = "/", const char* method = "GET", float version = 1.1);
	HttpRequest(const CString& str);

	CString &METHOD();
	CString &URL();
	CString &BODY();
	CString &operator [] (const CString& key);
	HttpRequest &operator () (const CString& strBody);
	CString &operator () ();

	HttpResponse Send();
};

class HttpWorker : public CThreadImpl<HttpWorker>
{
protected:
	int m_bClose;
	CSock m_sock;
public:

	typedef bool (*_MainLoop)(HttpResponse&, HttpRequest&);
	_MainLoop MainLoop;
	HttpWorker(const CSock& sock, _MainLoop fun)
	{
		m_sock = sock;
		MainLoop = fun;
	}

	void* Run()
	{
		cout << "Accept!" << m_tid << endl;
		CString buff;
		m_sock.SetRecvFlag(0);
		while (1)
		{
			char * p = m_sock.ReadN(1024);
			if (!p)
				break;
			buff += p;
			if (-1 != buff.Find("\r\n\r\n"))
			{
				//cout << buff << endl;
				cout << "[tid: " << m_tid << "] ";
				cout << "write hello" << endl;

				HttpRequest req(buff);
				HttpResponse resp(200);
				if (!this->MainLoop(resp, req))
					break;
				CString strResp = resp();
				cout << strResp << endl;
				m_sock.WriteString(strResp);
				buff = "";
			}
		}
		cout << "exit sub-thread!" << m_tid << endl;
		delete this;
	}
};


class HttpServer
{
	HttpWorker::_MainLoop MainLoop;
	short m_nPort;
public:
	HttpServer(short port, HttpWorker::_MainLoop fun) : m_nPort(port)
	{
		MainLoop = fun;
	}

	void Startup()
	{
		CServSock server(m_nPort);
		cout << "Http server start! [" << m_nPort << "]" << endl;
		while (1)
		{
			HttpWorker* worker = new HttpWorker(server.Accept(), MainLoop);
			worker->Start();
		}
	}
};


#endif
