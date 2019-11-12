#include "dc.h"
#include "wnd.h"
#include "string.h"

//	m_pImg = new char[m_nWidth * m_nHeight * 4];
//	m_pXImg = XCreateImage(g_dsp, CopyFromParent, depth, ZPixmap, 0, m_pImg, m_nWidth, m_nHeight, 32, m_nWidth*4);
//	XImage *img = XGetImage(g_dsp, m_hPixmap, 0, 0, m_nWidth, m_nHeight, 0xffffffff, ZPixmap);
//	XPutImage(g_dsp, m_hWnd, m_gc, img, 0, 0, 0, 0, m_nWidth, m_nHeight);
//	img->f.destroy_image(img);

CX11DC::CX11DC()
{
	m_hPixmap = 0;
	m_gc = nullptr;
	m_pImgBuff = nullptr;
}

CX11DC::CX11DC(Window wnd)
{
	m_hPixmap = 0;
	m_pImgBuff = nullptr;
	Init(wnd);
}

CX11DC::~CX11DC()
{
	if (m_hPixmap)
		XFreePixmap(X11_DSP, m_hPixmap);
	if (m_gc)
		XFreeGC(X11_DSP, m_gc);
	if (m_pImgBuff)
		m_pImgBuff->f.destroy_image(m_pImgBuff);
}

void CX11DC::Init(Window wnd)
{
	if (m_hPixmap)
		return;
	m_hPixmap = wnd;
	Window root;
	int x, y;
	unsigned border_width;
	XGetGeometry(X11_DSP, m_hPixmap, &root, &x, &y, &m_nWidth, &m_nHeight, &border_width, &m_nDepth);
	m_gc = XCreateGC(X11_DSP, m_hPixmap, 0, nullptr);
}

void CX11DC::CreateCompatible(const CX11DC &dc, unsigned w, unsigned h)
{
	if (w == 0)
		w = dc.m_nWidth;
	if (h == 0)
		h = dc.m_nHeight;
	m_nWidth = w;
	m_nHeight = h;
	m_nDepth = dc.m_nDepth;
	m_hPixmap = XCreatePixmap(X11_DSP, dc.m_hPixmap, w, h, dc.m_nDepth);
	m_gc = XCreateGC(X11_DSP, m_hPixmap, 0, nullptr);
}

void CX11DC::SelectPen(unsigned long color)
{
	XSetForeground(X11_DSP, m_gc, color);
}

void CX11DC::Rectangle(int x, int y, unsigned W, unsigned H)
{
	XDrawRectangle(X11_DSP, m_hPixmap, m_gc, x, y, W, H);
}

void CX11DC::TextOut(const char *str, int x, int y, int W, int H)
{
	if (str == nullptr)
		return;
	XFontStruct* font = X11_GUI(m_fontDefault);
	int string_height = font->ascent + font->descent;
	int n = int(strlen(str));
	int lines = 1;
	for (int i = 0; i < n; ++i)
	{
		if (str[i] == '\n')
			++lines;
	}
	if (lines > 1)
	{
		char* tmp = new char[n + 1];
		strcpy(tmp, str);
		int h_beg = (H - string_height * lines) / 2 + string_height + y;
		for (int i = 0, s = 0; i <= n; ++i)
		{
			if (str[i] == '\n' || str[i] == 0)
			{
				tmp[i] = 0;
				TextOut(&tmp[s], x, h_beg-string_height, W, string_height);
				h_beg += string_height;
				s = i + 1;
			}
		}
		delete[] tmp;
		return;
	}
	int remain_width = XTextWidth(font, "...", 3);
	int string_width = XTextWidth(font, str, n);
	if (string_width < W)
	{
		XDrawString(X11_DSP, m_hPixmap, m_gc, (W - string_width) / 2 + x, (H + string_height) / 2 + y, str, n);
		return;
	}
	int real_width;
	while (string_width > W)
	{
		--n;
		real_width = XTextWidth(font, str, n);
		string_width = real_width + remain_width;
	}
	XDrawString(X11_DSP, m_hPixmap, m_gc, x, (H + string_height) / 2 + y, str, n);
	XDrawString(X11_DSP, m_hPixmap, m_gc, x + real_width, (H + string_height) / 2 + y, "...", 3);
}

void CX11DC::Point(int x, int y, COLORREF clr)
{
	XSetForeground(X11_DSP, m_gc, clr);
	XDrawPoint(X11_DSP, m_hPixmap, m_gc, x, y);
}

void CX11DC::Line(int x1, int y1, int x2, int y2, COLORREF clr)
{
	XSetForeground(X11_DSP, m_gc, clr);
	XDrawLine(X11_DSP, m_hPixmap, m_gc, x1, y1, x2, y2);
}

#define EP_RGN(x, y, x1, y1, x2, y2) \
if (x < rcRgn.left) {\
	if (deltaX == 0) return;\
	y = (rcRgn.left - x1) * deltaY / deltaX + y1;\
	x = rcRgn.left;\
} else if (x > rcRgn.right) {\
	if (deltaX == 0) return;\
	y = (rcRgn.right - x1) * deltaY / deltaX + y1;\
	x = rcRgn.right;\
}\
if (y < rcRgn.top) {\
	if (deltaY == 0) return;\
	x = (rcRgn.top - y1) * deltaX / deltaY + x1;\
	y = rcRgn.top;\
} else if (y > rcRgn.bottom) {\
	if (deltaY == 0) return;\
	x = (rcRgn.bottom - y1) * deltaX / deltaY + x1;\
	y = rcRgn.bottom;\
}
void CX11DC::Line(const RECT rcRgn, int x1, int y1, int x2, int y2, COLORREF clr)
{
	int _x1 = x1, _x2 = x2, _y1 = y1, _y2 = y2;
	auto deltaX = x2 - x1;
	auto deltaY = y2 - y1;
	EP_RGN(_x1, _y1, x1, y1, x2, y2);
	EP_RGN(_x2, _y2, x1, y1, x2, y2);
	if (_x1 == _x2 && _y1 == _y2)
		return;
	XSetForeground(X11_DSP, m_gc, clr);
	XDrawLine(X11_DSP, m_hPixmap, m_gc, _x1, _y1, _x2, _y2);
}

void CX11DC::Rectangle(const RECT rcRgn, const RECT rcDraw, COLORREF clrBorder, COLORREF clrBKG)
{
	XSetForeground(X11_DSP, m_gc, clrBKG);
	XFillRectangle(X11_DSP, m_hPixmap, m_gc, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());
	XSetForeground(X11_DSP, m_gc, clrBorder);
	XDrawRectangle(X11_DSP, m_hPixmap, m_gc, rcDraw.left, rcDraw.top, rcDraw.Width() - 1, rcDraw.Height() - 1);
}

void CX11DC::TextStd(const RECT rcRgn, const RECT rcDraw, const char *str, COLORREF clr)
{
	XSetForeground(X11_DSP, m_gc, clr);
	TextOut(str, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());
}

void CX11DC::Text(const RECT rcRgn, const RECT rcDraw, const char *str, COLORREF clr, unsigned nFormat)
{
	XSetForeground(X11_DSP, m_gc, clr);
	TextOut(str, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());
}

void CX11DC::BitBlt(const ILiteDC &dc_src, int src_x, int src_y, unsigned w, unsigned h, int des_x, int des_y)
{
	const CX11DC* p = dynamic_cast<const CX11DC*>(&dc_src);
	if (p)
		XCopyArea(X11_DSP, p->m_hPixmap, m_hPixmap, m_gc, src_x, src_y, w, h, des_x, des_y);
}

char *CX11DC::BeginData(int x, int y, unsigned w, unsigned h)
{
	if (m_pImgBuff)
		m_pImgBuff->f.destroy_image(m_pImgBuff);
	m_nImgBuffX = x;
	m_nImgBuffY = y;
	m_nImgBuffW = w;
	m_nImgBuffH = h;
	m_pImgBuff = XGetImage(X11_DSP, m_hPixmap, x, y, w, h, 0xffffffff, ZPixmap);
	return m_pImgBuff->data;
}

void CX11DC::EndData()
{
	if (m_pImgBuff)
	{
		XPutImage(X11_DSP, m_hPixmap, m_gc, m_pImgBuff, 0, 0, m_nImgBuffX, m_nImgBuffY, m_nImgBuffW, m_nImgBuffH);
		m_pImgBuff->f.destroy_image(m_pImgBuff);
		m_pImgBuff = nullptr;
	}
}
