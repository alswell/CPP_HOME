#include "ZoomKLine.h"

void CZoomKLine::Add(float begin, float end, float low, float high)
{
	begin *= -100;
	end   *= -100;
	low   *= -100;
	high  *= -100;
	COLORREF clr = end >= begin ? CLR_G : CLR_R;
	int x = m_lsLine.size() * 10;
	if (m_posRT.y > high)
		m_posRT.y = high;
	m_posRT.x = x;
	auto pLine = new FigureLine{};
	pLine->line.push_back(PointF(x, low));
	pLine->line.push_back(PointF(x, high));
	pLine->clr = clr;
	m_lsLine.push_back(pLine);

	auto pRect = new FigureRect{};
	pRect->rc.left = x - 2;
	pRect->rc.right = x + 2;
	pRect->rc.bottom = end >= begin ? end : begin;
	pRect->rc.top = end >= begin ? begin : end;
	pRect->clr = CLR_NONE;
	pRect->clr_fill = clr;
	m_lsRect.push_back(pRect);
}
