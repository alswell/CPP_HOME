#ifndef __HTTP_H
#define __HTTP_H
#include "thread.h"
#include "sock.h"
#include "json.h"


class HttpResponse
{
	CSock m_sock;
	CStr m_strResp;
	CStr m_strVersion;
	int m_nStatus;
	CStr m_strStatus;
	CStr m_strBody;
	DICT(CStr) m_dHead;
public:
	HttpResponse(int status = 200, float version = 1.1);
	HttpResponse(CSock _sock);

	CStr &operator [] (const CStr& key);
	HttpResponse &operator () (const CStr& strBody);
	CStr &operator () ();
};

class HttpRequest
{
	CSock m_sock;
	CStr m_strReq;
	CStr m_strMethod;
	CStr m_strUrl;
	CStr m_strBody;
	float m_fVersion;
	DICT(CStr) m_dHead;
public:
	HttpRequest(const char* host, int port = 80, const char* url = "/", const char* method = "GET", float version = 1.1);
	HttpRequest(const CStr& str);

	CStr &METHOD();
	CStr &URL();
	CStr &BODY();
	CStr &operator [] (const CStr& key);
	HttpRequest &operator () (const CStr& strBody);
	CStr &operator () ();

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
		CStr buff;
		while (1)
		{
			char * p = m_sock.Read();
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
				CStr strResp = resp();
				cout << strResp << endl;
				m_sock.Write(strResp);
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
