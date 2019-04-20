#pragma once
#include <X11/Xlib.h>
#include "LiteCtrlBase.h"

class CWnd;
class CDC : public CLiteDCInterface
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
	CDC();
	CDC(Window wnd);
	virtual ~CDC();
	void Init(Window wnd);
	void CreateCompatible(const CDC& dc, unsigned w = 0, unsigned h = 0);
	void SelectPen(unsigned long color);
	void Rectangle(int x, int y, int W, int H);
	void TextOut(const char* str, int x, int y, int W, int H);

	virtual void Point(int x, int y, COLORREF clr);
	virtual void Line(int x1, int y1, int x2, int y2, COLORREF clr);
	virtual void Rectangle(RECT rcRgn, RECT rcDraw, COLORREF clrBorder, COLORREF clrBKG);
	virtual void TextStd(RECT rcRgn, RECT rcDraw, char * str, COLORREF clr);
	virtual void Text(RECT rcRgn, RECT rcDraw, char * str, COLORREF clr, unsigned nFormat);
	virtual void BitBlt(const CLiteDCInterface& dc_src, int src_x, int src_y, unsigned w, unsigned h, int des_x, int des_y);
	virtual char* BeginData(int x, int y, unsigned w, unsigned h);
	virtual void EndData();

};

