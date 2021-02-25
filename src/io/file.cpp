#include "file.h"

//int fileno(FILE * stream)
//FILE * fdopen(int fd, const char * mode)

CFile::CFile(const char *strFileName, const char* mode)
{
	m_pFile = fopen(strFileName, mode);
}

unsigned CFile::TotalSize()
{
	auto tmp = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_END);
	unsigned nSize = ftell(m_pFile);
	//rewind(m_pFile);
	fseek(m_pFile, tmp, SEEK_SET);
	return nSize;
}

char *CFile::ReadAll()
{
	return ReadN(TotalSize());
}

int CFile::Read(void *pBuff, unsigned nSize)
{
	return fread(pBuff, 1, nSize, m_pFile);
}

int CFile::Write(const void *pBuff, unsigned long nSize)
{
	return fwrite(pBuff, 1, nSize, m_pFile);
}

void CFile::Close()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = nullptr;
	}
}

list<CString> CFile::ReadLines()
{
	fseek(m_pFile, 0, SEEK_END);
	unsigned nSize = ftell(m_pFile);
	rewind(m_pFile);
	CString strBuff;
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


