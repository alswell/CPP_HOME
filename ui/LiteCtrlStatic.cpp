#include "LiteCtrlStatic.h"

CColorBlock::CColorBlock(COLORREF clrBkgnd, COLORREF clrBorder)
	: m_clrBkgnd(clrBkgnd)
	, m_clrBorder(clrBorder)
{
}

CColorBlock::~CColorBlock()
{
}

void CColorBlock::Draw(ILiteDC *dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	dc->Rectangle(rcViewRgn, rcLoc, m_clrBorder, m_clrBkgnd);
	//__super::Draw(ptOffset, rcParentRgn);
}

void CColorBlock::SetColor(COLORREF clrBkgnd, COLORREF clrBorder)
{
	m_clrBorder = clrBorder;
	m_clrBkgnd = clrBkgnd;
}

#define COPY_STRING(des, src) \
if (des) delete[] des;\
des = new char[strlen(src) + 1];\
strcpy(des, src)

CTextBlock::CTextBlock(const char* strText, unsigned nFormat, COLORREF clrText)
	: m_clrText(clrText)
{
	m_nFormat = nFormat;
	m_strText = nullptr;
	if (strText)
	{
		COPY_STRING(m_strText, strText);
	}
}

CTextBlock::~CTextBlock()
{
	if (m_strText)
		delete[] m_strText;
}

void CTextBlock::Draw(ILiteDC *dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	dc->Text(rcViewRgn, rcLoc, m_strText, m_clrText, m_nFormat);
}

void CTextBlock::SetColor(COLORREF clrText)
{
	m_clrText = clrText;
}

void CTextBlock::SetFont(unsigned nFont)
{
}

void CTextBlock::SetText(const char* str)
{
	COPY_STRING(m_strText, str);
	InvalidateCtrl();
}

void CTextBlock::SetText(int i)
{

}

void CTextBlock::SetText(float f)
{

}

void CTextBlock::SetText(double d)
{
}
