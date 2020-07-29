#pragma once
#include <X11/Xlib.h>
#include "LiteCtrlBase.h"

class CX11Wnd;
class CX11DC : public ILiteDC
{
	Pixmap m_hPixmap;
	unsigned m_nWidth;
	unsigned m_nHeight;
	unsigned m_nDepth;

	GC m_gc;
	XImage *m_pImgBuff;
	int m_nImgBuffX;
	int m_nImgBuffY;
	unsigned m_nImgBuffW;
	unsigned m_nImgBuffH;
public:
	CX11DC();
	CX11DC(Window wnd);
	virtual ~CX11DC();
	void Init(Window wnd);
	void SelectPen(unsigned long color);
	void Rectangle(int x, int y, unsigned W, unsigned H);
	void TextOut(const char* str, int x, int y, int W, int H);

	virtual ILiteDC* NewCopy();
	virtual void Point(int x, int y, COLORREF clr);
	virtual void Line(int x1, int y1, int x2, int y2, COLORREF clr);
	virtual void Line(const RECT rcRgn, int x1, int y1, int x2, int y2, COLORREF clr);
	virtual void Rectangle(const RECT rcRgn, const RECT rcDraw, COLORREF clrBorder, COLORREF clrBKG);
	virtual void TextStd(const RECT rcRgn, const RECT rcDraw, const char* str, COLORREF clr);
	virtual void Text(const RECT rcRgn, const RECT rcDraw, const char* str, COLORREF clr, unsigned nFormat);
	virtual void BitBlt(const ILiteDC& dc_src, int src_x, int src_y, unsigned w, unsigned h, int des_x, int des_y);
	virtual char* BeginData(int x, int y, unsigned w, unsigned h);
	virtual void EndData();

};

