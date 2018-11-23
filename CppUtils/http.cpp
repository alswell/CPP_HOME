#include "http.h"

HttpResponse::HttpResponse(int status, float version)
{
	m_strVersion.Format("HTTP/%.1f", version);
	m_nStatus = status;
	m_strStatus = "OK";
}

HttpResponse::HttpResponse(CSock _sock)
{
	m_sock = _sock;
	int n = -1;
	int total = 0;
	m_sock.SetRecvFlag(0);
	while (1)
	{
		char * p = m_sock.ReadN(1024);
		if (!p)
			return;
		m_strResp += p;
		n = m_strResp.Find("\r\n\r\n");
		if (n != -1)
		{
			list<CString> ls = m_strResp.Left(n).Split("\r\n");
			list<CString> lsStatus = ls.front().Split(' ');
			m_strVersion = lsStatus.front();
			lsStatus.pop_front();
			m_nStatus = atoi(lsStatus.front());
			lsStatus.pop_front();
			m_strStatus = lsStatus.front();
			ls.pop_front();
			FOR_LIST(CString, ls, it)
			{
				list<CString> kv = it->Split(':', 1);
				m_dHead[kv.front()] = kv.back().Trim();
			}
			total = atoi(m_dHead["Content-Length"]) + n + 4;
		}
		if (total && m_strResp.GetLength() >= total)
			break;
	}
	m_strBody = m_strResp.Mid(n + 4);
}

CString &HttpResponse::operator [](const CString &key)
{
	if (key == "body")
		return m_strBody;
	return m_dHead[key];
}

CString &HttpResponse::operator ()()
{
	m_strResp.Format("%s %d %s\r\n", (const char*)m_strVersion, m_nStatus, (const char*)m_strStatus);
	FOR_DICT(CString, m_dHead, it)
	{
		m_strResp.AppendFormat("%s: %s\r\n", (const char*)(it->first), (const char*)(it->second));
	}
	m_strResp += "\r\n";
	m_strResp += m_strBody;
}

HttpResponse &HttpResponse::operator ()(const CString &strBody)
{
	m_strBody = strBody;
	return *this;
}


HttpRequest::HttpRequest(const char *host, int port, const char *url, const char *method, float version)
	: m_fVersion(version)
{
	m_sock.Connect(host, port);
	m_dHead["Host"].Format("%s:%d", host, port);
	m_strMethod = method;
	m_strUrl = url;
	m_sock.SetTimeout(5);
}

HttpRequest::HttpRequest(const CString &str)
{
	m_strReq = str;
	int n = m_strReq.Find("\r\n\r\n");
	if (n == -1)
		return;

	list<CString> ls = m_strReq.Left(n).Split("\r\n");
	list<CString> lsStatus = ls.front().Split(' ');
	m_strMethod = lsStatus.front();
	lsStatus.pop_front();
	m_strUrl = lsStatus.front();
	lsStatus.pop_front();
	m_fVersion = atof(lsStatus.front().Mid(5));
	ls.pop_front();
	FOR_LIST(CString, ls, it)
	{
		list<CString> kv = it->Split(':', 1);
		m_dHead[kv.front()] = kv.back().Trim();
	}
	m_strBody = m_strReq.Mid(n + 4);
}

CString &HttpRequest::METHOD()
{
	return m_strMethod;
}

CString &HttpRequest::URL()
{
	return m_strUrl;
}

CString &HttpRequest::BODY()
{
	return m_strBody;
}

CString &HttpRequest::operator [](const CString &key)
{
	return m_dHead[key];
}

HttpRequest &HttpRequest::operator ()(const CString &strBody)
{
	m_strBody = strBody;
	return *this;
}

CString &HttpRequest::operator ()()
{
	m_strReq.Format("%s %s HTTP/%.1f\r\n", (const char*)m_strMethod, (const char*)m_strUrl, m_fVersion);
	FOR_DICT(CString, m_dHead, it)
	{
		m_strReq.AppendFormat("%s: %s\r\n", (const char*)(it->first), (const char*)(it->second));
	}
	m_strReq += "\r\n";
	m_strReq += m_strBody;
}

HttpResponse HttpRequest::Send()
{
	m_sock.WriteString((*this)());
	return HttpResponse(m_sock);
}
