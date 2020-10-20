#include "stream.h"

IStream::IStream()
{
	m_pBuff = nullptr;
	m_nSize = 0;
}

IStream::~IStream()
{
	if (m_pBuff)
	{
		delete[] m_pBuff;
		m_pBuff = nullptr;
	}
	//Close();
}

char *IStream::ReadN(unsigned nSize)
{
	if (m_nSize < nSize)
	{
		if (m_pBuff)
			delete[] m_pBuff;
		m_nSize = nSize;
		m_pBuff = new char[m_nSize];
	}
	int r = Read(m_pBuff, nSize);
	if (r < 0)
		return nullptr;
	m_pBuff[r] = 0;
	return m_pBuff;
}

char *IStream::ReadLine(unsigned nSize)
{
	if (m_nSize < nSize)
	{
		if (m_pBuff)
			delete[] m_pBuff;
		m_nSize = nSize;
		m_pBuff = new char[m_nSize];
	}
	unsigned i = 0;
	for (;; ++i)
	{
		if (i == m_nSize)
		{
			auto buff = new char[m_nSize + nSize];
			memcpy(buff, m_pBuff, m_nSize);
			delete[] m_pBuff;
			m_pBuff = buff;
			m_nSize += nSize;
		}
		int r = Read(&m_pBuff[i], 1);
		switch (r) {
		case 1:
			if (m_pBuff[i] == '\n')
			{
				m_pBuff[i] = 0;
				return m_pBuff;
			}
			break;
		case 0:
			cout << "ELF" << endl;
			m_pBuff[i] = 0;
			return m_pBuff;
		default:
			return nullptr;
		}
	}
	m_pBuff[i] = 0;
	return m_pBuff;
}

int IStream::WriteString(const char *str)
{
	return Write(str, strlen(str));
}

int IStream::WriteLine(const char *str)
{
	auto nString = strlen(str);
	auto nLine = nString + strlen(CRLF);
	char* buff = new char[nLine + 1];
	strcpy(buff, str);
	strcpy(&buff[nString], CRLF);
	int r = Write(buff, nLine);
	delete[] buff;
	return r;
//	int r = Write(str, strlen(str));
//	return WriteString(CRLF) == strlen(CRLF) ? r : -1;
}

void IStream::WithExit()
{
	Close();
}
