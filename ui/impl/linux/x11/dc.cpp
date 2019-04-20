#include "dc.h"
#include "wnd.h"
#include "string.h"

//	m_pImg = new char[m_nWidth * m_nHeight * 4];
//	m_pXImg = XCreateImage(g_dsp, CopyFromParent, depth, ZPixmap, 0, m_pImg, m_nWidth, m_nHeight, 32, m_nWidth*4);
//	XImage *img = XGetImage(g_dsp, m_hPixmap, 0, 0, m_nWidth, m_nHeight, 0xffffffff, ZPixmap);
//	XPutImage(g_dsp, m_hWnd, m_gc, img, 0, 0, 0, 0, m_nWidth, m_nHeight);
//	img->f.destroy_image(img);

CDC::CDC()
{
	m_hPixmap = 0;
	m_gc = NULL;
	m_pImgBuff = NULL;
}

CDC::CDC(Window wnd)
{
	m_hPixmap = 0;
	m_pImgBuff = NULL;
	Init(wnd);
}

CDC::~CDC()
{
	if (m_hPixmap)
		XFreePixmap(g_dsp, m_hPixmap);
	if (m_gc)
		XFreeGC(g_dsp, m_gc);
	if (m_pImgBuff)
		m_pImgBuff->f.destroy_image(m_pImgBuff);
}

class CFontMgr
{
	XFontStruct* m_fontDefault;
public:
	CFontMgr()
	{
		m_fontDefault = XLoadQueryFont(g_dsp, "-misc-fixed-medium-r-semicondensed--0-0-75-75-c-0-iso8859-9");
	}
	XFontStruct* Default()
	{
		return m_fontDefault;
	}
	static CFontMgr& Instance()
	{
		static CFontMgr* p = NULL;
		if (p == NULL)
			p = new CFontMgr;
		return *p;
	}
};


void CDC::Init(Window wnd)
{
	if (m_hPixmap)
		return;
	m_hPixmap = wnd;
	Window root;
	int x, y;
	unsigned border_width;
	XGetGeometry(g_dsp, m_hPixmap, &root, &x, &y, &m_nWidth, &m_nHeight, &border_width, &m_nDepth);
	m_gc = XCreateGC(g_dsp, m_hPixmap, 0, NULL);
}

void CDC::CreateCompatible(const CDC &dc, unsigned w, unsigned h)
{
	if (w == 0)
		w = dc.m_nWidth;
	if (h == 0)
		h = dc.m_nHeight;
	m_nWidth = w;
	m_nHeight = h;
	m_nDepth = dc.m_nDepth;
	m_hPixmap = XCreatePixmap(g_dsp, dc.m_hPixmap, w, h, dc.m_nDepth);
	m_gc = XCreateGC(g_dsp, m_hPixmap, 0, NULL);
}

void CDC::SelectPen(unsigned long color)
{
	XSetForeground(g_dsp, m_gc, color);
}

void CDC::Rectangle(int x, int y, int W, int H)
{
	XDrawRectangle(g_dsp, m_hPixmap, m_gc, x, y, W, H);
}

void CDC::TextOut(const char *str, int x, int y, int W, int H)
{
	XFontStruct* font = CFontMgr::Instance().Default();
	int string_height = font->ascent + font->descent;
	int n = strlen(str);
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
		XDrawString(g_dsp, m_hPixmap, m_gc, (W - string_width) / 2 + x, (H + string_height) / 2 + y, str, n);
		return;
	}
	int real_width;
	while (string_width > W)
	{
		--n;
		real_width = XTextWidth(font, str, n);
		string_width = real_width + remain_width;
	}
	XDrawString(g_dsp, m_hPixmap, m_gc, x, (H + string_height) / 2 + y, str, n);
	XDrawString(g_dsp, m_hPixmap, m_gc, x + real_width, (H + string_height) / 2 + y, "...", 3);
}

void CDC::Point(int x, int y, COLORREF clr)
{
	XSetForeground(g_dsp, m_gc, clr);
	XDrawPoint(g_dsp, m_hPixmap, m_gc, x, y);
}

void CDC::Line(int x1, int y1, int x2, int y2, COLORREF clr)
{
	XSetForeground(g_dsp, m_gc, clr);
	XDrawLine(g_dsp, m_hPixmap, m_gc, x1, y1, x2, y2);
}

void CDC::Rectangle(RECT rcRgn, RECT rcDraw, COLORREF clrBorder, COLORREF clrBKG)
{
	XSetForeground(g_dsp, m_gc, clrBKG);
	XFillRectangle(g_dsp, m_hPixmap, m_gc, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());
	XSetForeground(g_dsp, m_gc, clrBorder);
	XDrawRectangle(g_dsp, m_hPixmap, m_gc, rcDraw.left, rcDraw.top, rcDraw.Width() - 1, rcDraw.Height() - 1);
}

void CDC::TextStd(RECT rcRgn, RECT rcDraw, char *str, COLORREF clr)
{
	XSetForeground(g_dsp, m_gc, clr);
	TextOut(str, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());
}

void CDC::Text(RECT rcRgn, RECT rcDraw, char *str, COLORREF clr, unsigned nFormat)
{
	XSetForeground(g_dsp, m_gc, clr);
	TextOut(str, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());
}

void CDC::BitBlt(const CLiteDCInterface &dc_src, int src_x, int src_y, unsigned w, unsigned h, int des_x, int des_y)
{
	const CDC* p = dynamic_cast<const CDC*>(&dc_src);
	if (p)
		XCopyArea(g_dsp, p->m_hPixmap, m_hPixmap, m_gc, src_x, src_y, w, h, des_x, des_y);
}

char *CDC::BeginData(int x, int y, unsigned w, unsigned h)
{
	if (m_pImgBuff)
		m_pImgBuff->f.destroy_image(m_pImgBuff);
	m_nImgBuffX = x;
	m_nImgBuffY = y;
	m_nImgBuffW = w;
	m_nImgBuffH = h;
	m_pImgBuff = XGetImage(g_dsp, m_hPixmap, x, y, w, h, 0xffffffff, ZPixmap);
	return m_pImgBuff->data;
}

void CDC::EndData()
{
	if (m_pImgBuff)
	{
		XPutImage(g_dsp, m_hPixmap, m_gc, m_pImgBuff, 0, 0, m_nImgBuffX, m_nImgBuffY, m_nImgBuffW, m_nImgBuffH);
		m_pImgBuff->f.destroy_image(m_pImgBuff);
		m_pImgBuff = NULL;
	}
}
