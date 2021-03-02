#pragma once
#include "../thread/thread.h"
#include "../io/sock.h"
#include "../str.h"
//#include "../dict.h"


class HttpResponse
{
	friend struct InitHTTP;
	static map<int, const char*> m_mStatus;
	int m_nStatus;
	float m_fVersion;
	map<CString, CString> m_mHeader;
	int m_nContentLength;
	char* m_bufBody;
public:
	HttpResponse(int status = 200, float version = 1.1);
	HttpResponse(IStream& reader);

	void SetBody(const void *body, int len);
	void SetBodyJPG(const void *body, int len);
	void NotFound();

	CString &operator [] (const CString& key);
	char* BODY();
	int BodyLen();

	CString Head();
};

enum HttpMethod
{
	METHOD_UNKNOWN = 0,
	METHOD_GET,
	METHOD_DELETE,
	METHOD_PUT,
	METHOD_POST,
};


class HttpRequest
{
	friend struct InitHTTP;
	friend class HttpWorker;
	static map<HttpMethod, const char*> m_mMethod;
	HttpMethod m_eMethod;
	float m_fVersion;
	CString m_strUrl;
	map<CString, CString> m_mHeader;
	int m_nContentLength;
	const char* m_strHost;
	const char*  m_strUserAgent;
	const char*  m_strAccept;
	const char*  m_strContentType;
	char* m_bufBody;
public:
	HttpRequest();
	HttpRequest(const char* host, int port = 80, const char* url = "/", HttpMethod method = METHOD_GET, float version = 1.1);
	bool Init(IStream& reader);

	HttpMethod &METHOD();
	CString &URL();
	char* BODY();
	CString &operator [] (const CString& key);

	CString Head();
};

#define LOG cout << "[" << m_tid << "] "
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
		while (true)
		{
			HttpRequest req;
			if (!req.Init(m_sock))
				break;
			LOG << "UserAgent: " << req.m_strUserAgent << endl;
			LOG << "Version: " << req.m_fVersion << endl;
			LOG << "Method: " << HttpRequest::m_mMethod[req.METHOD()] << endl;
			LOG << "Host: " << req.m_strHost << endl;
			LOG << "URL: " << req.URL() << endl;
			LOG << "Content-Length: " << req.m_nContentLength << endl;
			LOG << "Content-Type: " << req.m_strContentType << endl;
			LOG << "Accept: " << req.m_strAccept << endl;
			LOG << "Body: " << req.BODY() << endl;
			HttpResponse resp;
			this->MainLoop(resp, req);
			if (req.m_fVersion == 1.0)
				break;
			m_sock.WriteString(resp.Head());
			m_sock.Write(resp.BODY(), resp.BodyLen());
		}
		m_sock.Close();
		LOG << "exit!" << endl;
		delete this;
		return nullptr;
	}
};

bool default_http_handler(HttpResponse& resp, HttpRequest& req);

class HttpServer : public CThreadImpl<HttpServer>
{
	HttpWorker::_MainLoop MainLoop;
	short m_nPort;
public:
	HttpServer(short port, HttpWorker::_MainLoop fun=default_http_handler) : m_nPort(port)
	{
		MainLoop = fun;
	}

	void* Run()
	{
		CServSock server(m_nPort);
		cout << "Http server start! [" << m_nPort << "]" << endl;
		while (1)
		{
			HttpWorker* worker = new HttpWorker(server.Accept(), MainLoop);
			worker->Start();
		}
		return nullptr;
	}
};

