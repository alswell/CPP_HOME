#pragma once
#include "zoom/FigurePainter.h"


class CZoomFigure : public IZoom
{
	list<FigurePt*> m_lsPt;
	list<FigureLine*> m_lsLine;
public:
	CZoomFigure();
	virtual void Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn);
	virtual RECT GetRect();
	virtual void GetPixInfo(char *buff, int r, int c);

	FigurePt* AddPoint(float x, float y, COLORREF clr, const char* c);
	void Add(FigurePt* pt);
	void Add(FigureLine* line);
};

