#include "stream.h"

IStream::IStream()
{
	m_pBuff = NULL;
	m_nSize = 0;
}

IStream::~IStream()
{
	if (m_pBuff)
	{
		delete[] m_pBuff;
		m_pBuff = NULL;
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
		return NULL;
	m_pBuff[r] = 0;
	return m_pBuff;
}

char *IStream::ReadLine(int nSize)
{
	if (m_nSize < nSize)
	{
		if (m_pBuff)
			delete[] m_pBuff;
		m_nSize = nSize;
		m_pBuff = new char[m_nSize];
	}
	int i = 0;
	for (; i < m_nSize; ++i)
	{
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
			break;
		default:
			return NULL;
			break;
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
	int r = Write(str, strlen(str));
	return WriteString(CRLF) == strlen(CRLF) ? r : -1;
}
