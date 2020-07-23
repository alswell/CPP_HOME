#pragma once
#include "zoom/IZoom.h"
#include "StdMapper.h"

struct FigurePt
{
	PointF pt;
	COLORREF clr;
	const char* c;
	bool visible;
};

struct FigureLine
{
	list<PointF> line;
	COLORREF clr;
	const char* c;
	bool visible;
};

class CZoomFigure : public IZoom
{
	list<FigurePt*> m_lsPt;
	list<FigureLine*> m_lsLine;
public:
	CZoomFigure();
	virtual void Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn);
	virtual RECT GetRect();
	virtual void GetPixInfo(char *buff, int r, int c);

	void Add(FigurePt* pt);
	void Add(FigureLine* line);
};

class CPainHelper
{
	CZoomFigure* m_pZoom;
	ILiteDC* m_implDC;
	const RECT& m_rcLoc;
	const RECT& m_rcViewRgn;
public:
	CPainHelper(CZoomFigure* pZoom, ILiteDC* implDC, const RECT& m_rcLoc, const RECT& m_rcViewRgn);

	void PtRevert(PointF& pt);

	void Point(PointF pt0, COLORREF clr = CLR_DEFAULT, const char* c = "0");
	void Line(PointF pt1, PointF pt2, COLORREF clr = CLR_G);
	void Plot(const list<PointF>& ls, COLORREF clr = CLR_G, const char* c = nullptr);
};

