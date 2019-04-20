#pragma once
#include "LiteCtrlBase.h"


#define ADD_BLOCK(l, t, W, H, clrBKG, clrBorder) dynamic_cast<CColorBlock*>(AddCtrl(new CColorBlock(this, RECT(l, t, l+W, t+H), clrBKG, clrBorder)))
#define ADD_BLOCK2(rc, clrBKG, clrBorder) dynamic_cast<CColorBlock*>(AddCtrl(new CColorBlock(this, rc, clrBKG, clrBorder)))
class CColorBlock :	public CLiteCtrlBase
{
	COLORREF m_clrBkgnd;
	COLORREF m_clrBorder;
public:
	CColorBlock(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc, COLORREF clrBkgnd, COLORREF clrBorder = CLR_NONE);
	virtual ~CColorBlock();

	virtual void Draw(POINT ptOffset, RECT rcParentRgn);

	void SetColor(COLORREF clrBkgnd, COLORREF clrBorder);
};


#define ADD_TXT(l, t, W, H, str, fmt, clr) dynamic_cast<CTextBlock*>(AddCtrl(new CTextBlock(this, RECT(l, t, l+W, t+H), str, fmt, clr)))
class CTextBlock : public CLiteCtrlBase
{
	char* m_strText;
	COLORREF m_clrText;
	unsigned m_nFormat;
public:
	CTextBlock(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc, const char* strText, unsigned nFormat = STD_TXT_FMT, COLORREF clrText = CLR_DEFAULT);
	virtual ~CTextBlock();

	virtual void Draw(POINT ptOffset, RECT rcParentRgn);

	void SetColor(COLORREF clrText);
	void SetFont(unsigned nFont);

	void SetText(const char* str);
	void SetText(int i);
	void SetText(float f);
	void SetText(double d);
};
