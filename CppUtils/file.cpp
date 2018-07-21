#include "file.h"

//int fileno(FILE * stream)
//FILE * fdopen(int fd, const char * mode)

CFile::CFile(const char *strFileName, const char* mode)
{
	m_pFile = fopen(strFileName, mode);
}

int CFile::Read(void *pBuff, unsigned nSize)
{
	return fread(pBuff, 1, nSize, m_pFile);
}

int CFile::Write(const void *pBuff, unsigned nSize)
{
	return fwrite(pBuff, 1, nSize, m_pFile);
}

void CFile::Close()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

list<CStr> CFile::ReadLines()
{
	fseek(m_pFile, 0, SEEK_END);
	unsigned nSize = ftell(m_pFile);
	rewind(m_pFile);
	CStr strBuff;
	fread(strBuff.GetBuffer(nSize + 1), 1, nSize, m_pFile);
	strBuff.ReleaseBuffer();
	return strBuff.Split('\n');
}

//FILE *CFileIn::Attach(FILE *f)
//{
//	FILE* temp = m_pFile;
//	m_pFile = f;
//	return temp;
//}

//FILE *CFileIn::Detach()
//{
//	FILE* temp = m_pFile;
//	m_pFile = NULL;
//	return temp;
//}

//CFileIn CFileIn::OpenFile(const char *file, const char *mode)
//{
//	return CFileIn(fopen(file, mode));
//}

//CFileIn CFileIn::OpenPipe(const char *cmd, const char *mode)
//{
//	return CFileIn(popen(cmd, mode));
//}


