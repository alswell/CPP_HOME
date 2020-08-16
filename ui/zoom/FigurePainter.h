#pragma once
#include "IZoom.h"

#define FIGURE_BASE \
	COLORREF clr;\
	bool visible;

struct FigurePt
{
	PointF pt;
	char c;
	FIGURE_BASE
};

struct FigureLine
{
	list<PointF> line;
	char c;
	FIGURE_BASE
};

struct FigureRect
{
	CRect<float> rc;
	COLORREF clr_fill;
	FIGURE_BASE
};

class CPainHelper
{
	IBmpMapper* m_pZoom;
	ILiteDC* m_implDC;
	const RECT& m_rcLoc;
	const RECT& m_rcViewRgn;
public:
	CPainHelper(IBmpMapper* pZoom, ILiteDC* implDC, const RECT& m_rcLoc, const RECT& m_rcViewRgn);

	void PtRevert(float& x, float& y);
	void PtRevert(PointF& pt);

	void Point(PointF pt0, COLORREF clr = CLR_DEFAULT, char c = '*');
	void Point(PointF pt0, COLORREF clr = CLR_DEFAULT, const char* c = "0");
	void Line(PointF pt1, PointF pt2, COLORREF clr = CLR_G);
	void Rect(CRect<float> rc, COLORREF clr, COLORREF clr_fill);
	void Plot(const list<PointF>& ls, COLORREF clr = CLR_G, char c = 0);
};

