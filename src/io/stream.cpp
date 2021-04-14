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
}

char *IStream::ReadN(unsigned nSize)
{
	if (m_nSize < nSize)
	{
		if (m_pBuff)
			delete[] m_pBuff;
		m_nSize = nSize;
		m_pBuff = new char[m_nSize+1];
	}
	int r = Read(m_pBuff, nSize);
	if (r < 0)
		return nullptr;
	m_pBuff[r] = 0;
	return m_pBuff;
}

char *IStream::ReadUntil(const char *cutset, unsigned nSize)
{
	auto n = strlen(cutset);
	if (m_nSize < nSize)
	{
		if (m_pBuff)
			delete[] m_pBuff;
		m_nSize = nSize;
		m_pBuff = new char[m_nSize];
	}
	unsigned i = 1;
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
			for (unsigned j = 0; j < n; ++j)
			{
				if (m_pBuff[i] == cutset[j])
				{
					m_pBuff[0] = cutset[j];
					m_pBuff[i] = 0;
					return m_pBuff+1;
				}
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
	return m_pBuff+1;
}

char *IStream::ReadLine(unsigned nSize)
{
	return ReadUntil("\n", nSize);
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

void IStream::WithAutoClose()
{
	Close();
}
