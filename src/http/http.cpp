#include "http.h"

map<int, const char*> HttpResponse::m_mStatus;
map<HttpMethod, const char*> HttpRequest::m_mMethod;

struct InitHTTP {
	InitHTTP() {
		cout << "Init HTTP" << endl;
		HttpResponse::m_mStatus[200]  ="OK";
		HttpResponse::m_mStatus[404]  ="Not Found";

		HttpRequest::m_mMethod[METHOD_UNKNOWN] = "UNKNOWN";
		HttpRequest::m_mMethod[METHOD_GET] = "GET";
		HttpRequest::m_mMethod[METHOD_DELETE] = "DELETE";
		HttpRequest::m_mMethod[METHOD_PUT] = "PUT";
		HttpRequest::m_mMethod[METHOD_POST] = "POST";
	}
};
InitHTTP init_http;


HttpResponse::HttpResponse(int status, float version)
{
	m_fVersion = version;
	m_nStatus = status;
	m_nContentLength = 0;
	m_bufBody = nullptr;
	m_mHeader["Content-Length"] = "0";
}

HttpResponse::HttpResponse(IStream &reader)
{
	reader.ReadUntil(" ");
	m_nStatus = atoi(reader.ReadUntil(" "));
	reader.ReadUntil("\n");

	while (true)
	{
		auto p = reader.ReadUntil(":\n");
		if (*(p-1) == ':')
		{
			m_mHeader[p] = reader.ReadUntil("\r");
			reader.ReadUntil("\n");
			continue;
		}
		break;
	}
	m_nContentLength = atoi(m_mHeader["Content-Length"]);
	m_bufBody = new char[m_nContentLength+1];
	m_bufBody[m_nContentLength] = 0;
	reader.Read(m_bufBody, m_nContentLength);
}

void HttpResponse::SetBody(const void* body, int len)
{
	if (m_nContentLength < len)
	{
		m_nContentLength = len;
		m_mHeader["Content-Length"].Format("%d", m_nContentLength);
		if (m_bufBody)
			delete []m_bufBody;
		m_bufBody = new char[m_nContentLength+1];
	}
	if (m_bufBody)
		m_bufBody[m_nContentLength] = 0;
	memcpy(m_bufBody, body, len);
}

void HttpResponse::SetBodyJPG(const void *body, int len)
{
	m_mHeader["Content-Type"] = "image/jpg";
	SetBody(body, len);
}

void HttpResponse::NotFound()
{
	m_nStatus = 404;
	SetBody("not found", 9);
}

CString &HttpResponse::operator [](const CString &key)
{
	return m_mHeader[key];
}

char *HttpResponse::BODY()
{
	return m_bufBody;
}

int HttpResponse::BodyLen()
{
	return m_nContentLength;
}

CString HttpResponse::Head()
{
	CString m_strResp;
	m_strResp.Format("HTTP/%.1f %d %s\r\n", m_fVersion, m_nStatus, m_mStatus[m_nStatus]);
	for (auto it = m_mHeader.begin(); it != m_mHeader.end(); ++it)
		m_strResp.AppendFormat("%s: %s\r\n", (const char*)(it->first), (const char*)(it->second));
	m_strResp += "\r\n";
	return m_strResp;
}


HttpRequest::HttpRequest() : m_eMethod(METHOD_UNKNOWN), m_fVersion(0), m_nContentLength(0), m_bufBody(nullptr) {}

HttpRequest::HttpRequest(const char *host, int port, const char *url, HttpMethod method, float version)
	: m_fVersion(version)
{
	m_mHeader["Host"].Format("%s:%d", host, port);
	m_eMethod = method;
	m_strUrl = url;
}

bool HttpRequest::Init(IStream &reader)
{
	auto p = reader.ReadUntil(" "); if (!p) return false;
	for (auto it = m_mMethod.begin(); it != m_mMethod.end(); ++it) if (strcmp(it->second, p) == 0)
	{
		m_eMethod = it->first;
		break;
	}
	p = reader.ReadUntil(" "); if (!p) return false;
	m_strUrl = p;
	p = reader.ReadUntil("/"); if (!p) return false;
	p = reader.ReadUntil("\r"); if (!p) return false;
	m_fVersion = atof(p);
	p = reader.ReadUntil("\n"); if (!p) return false;

	while (true)
	{
		p = reader.ReadUntil(":\n"); if (!p) return false;
		if (*(p-1) == ':')
		{
			auto& v = m_mHeader[p];
			p = reader.ReadUntil("\r"); if (!p) return false;
			v = p+1;
			p = reader.ReadUntil("\n"); if (!p) return false;
			continue;
		}
		break;
	}
	m_nContentLength = atoi(m_mHeader["Content-Length"]);
	m_strHost = m_mHeader["Host"];
	m_strUserAgent = m_mHeader["User-Agent"];
	m_strAccept = m_mHeader["Accept"];
	m_strContentType = m_mHeader["Content-Type"];
	m_bufBody = new char[m_nContentLength+1];
	m_bufBody[m_nContentLength] = 0;
	return reader.Read(m_bufBody, m_nContentLength) == m_nContentLength;
}

HttpMethod &HttpRequest::METHOD()
{
	return m_eMethod;
}

CString &HttpRequest::URL()
{
	return m_strUrl;
}

char *HttpRequest::BODY()
{
	return m_bufBody;
}

CString &HttpRequest::operator [](const CString &key)
{
	return m_mHeader[key];
}

CString HttpRequest::Head()
{
	CString m_strReq;
	m_strReq.Format("%s %s HTTP/%.1f\r\n", m_mMethod[m_eMethod], (const char*)m_strUrl, m_fVersion);
	for (auto it = m_mHeader.begin(); it != m_mHeader.end(); ++it)
		m_strReq.AppendFormat("%s: %s\r\n", (const char*)(it->first), (const char*)(it->second));
	m_strReq += "\r\n";
	return m_strReq;
}

bool default_http_handler(HttpResponse &resp, HttpRequest &req)
{
	if (req.URL() == "/")
		resp.SetBody("hello", 5);
	else
		resp.NotFound();
	return true;
}
