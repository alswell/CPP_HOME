#pragma once
#include "zoom/IZoom.h"
#include "StdMapper.h"

#define ZOOM_NOTIFY_RECT 111

#define FIGURE_BASE \
	COLORREF clr;\
	bool visible;

struct FigurePt
{
	PointF pt;
	const char* c;
	FIGURE_BASE
};

struct FigureLine
{
	list<PointF> line;
	const char* c;
	FIGURE_BASE
};

struct FigureRect
{
	CRect<float> rc;
	FIGURE_BASE
};

class CZoomFigure : public IZoom
{
	list<FigurePt*> m_lsPt;
	list<FigureLine*> m_lsLine;
	list<FigureRect*> m_lsRect;

	PointF m_ptDown;
	FigureRect* m_pActiveRect;
public:
	CZoomFigure();
	virtual void Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn);
	virtual RECT GetRect();
	virtual void GetPixInfo(char *buff, int r, int c);

	FigurePt* AddPoint(float x, float y, COLORREF clr, const char* c);
	FigureRect* AddRect(float l, float t, float W, float H, COLORREF clr);
	void Add(FigurePt* pt);
	void Add(FigureLine* line);

	virtual void Activate(POINT ptWnd);
	virtual void ActivateMove(POINT ptWnd);
	virtual void Inactivate(bool bCapture);
};

class CPainHelper
{
	CZoomFigure* m_pZoom;
	ILiteDC* m_implDC;
	const RECT& m_rcLoc;
	const RECT& m_rcViewRgn;
public:
	CPainHelper(CZoomFigure* pZoom, ILiteDC* implDC, const RECT& m_rcLoc, const RECT& m_rcViewRgn);

	void PtRevert(float& x, float& y);
	void PtRevert(PointF& pt);

	void Point(PointF pt0, COLORREF clr = CLR_DEFAULT, const char* c = "0");
	void Line(PointF pt1, PointF pt2, COLORREF clr = CLR_G);
	void Rect(CRect<float> rc, COLORREF clr);
	void Plot(const list<PointF>& ls, COLORREF clr = CLR_G, const char* c = nullptr);
};

