#pragma once
#include "zoom/FigurePainter.h"


class CZoomFigure : public IZoom
{
protected:
	list<FigurePt*> m_lsPt;
	list<FigureLine*> m_lsLine;
	list<FigureRect*> m_lsRect;
public:
	CZoomFigure();
	virtual void Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn) override;
	virtual RECT GetRect() override;
	virtual void GetPixInfo(char *buff, int r, int c) override;

	FigurePt* AddPoint(float x, float y, COLORREF clr, char c);
	FigureRect* AddRect(float l, float t, float W, float H, COLORREF clr);
	void Add(FigurePt* pt);
	void Add(FigureLine* line);
};

