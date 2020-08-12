#pragma once
#include "ZoomFigure.h"

class CZoomEditable : public CZoomFigure
{
	PointF m_ptDown;
	FigureRect* m_pActiveRect;
public:
	virtual void Activate(POINT ptWnd);
	virtual void ActivateMove(POINT ptWnd);
	virtual void Inactivate(bool bCapture);
};
