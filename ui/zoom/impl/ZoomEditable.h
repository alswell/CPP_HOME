#pragma once
#include "zoom/FigurePainter.h"

class CZoomEditable : public IZoom
{
//	list<FigurePt*> m_lsPt;
//	list<FigureLine*> m_lsLine;
	list<FigureRect*> m_lsRect;

	PointF m_ptDown;
	FigureRect* m_pActiveRect;
public:
	CZoomEditable();
	virtual void Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn);
	virtual RECT GetRect();
	virtual void GetPixInfo(char *buff, int r, int c);

	virtual void Activate(POINT ptWnd);
	virtual void ActivateMove(POINT ptWnd);
	virtual void Inactivate(bool bCapture);
};
