#include "ZoomFigure.h"

CZoomFigure::CZoomFigure()
{
	m_bHyperCtrl = true;
}

void CZoomFigure::Draw(ILiteDC *dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	CPainHelper helper(this->m_implBmpMapper, dc, rcLoc, rcViewRgn);
	char c[] = "A";
	FOR_EACH(itLine, m_lsLine)
	{
		if ((**itLine).c != nullptr)
		{
			helper.Plot((**itLine).line, (**itLine).clr, (**itLine).c);
		}
		else
		{
			helper.Plot((**itLine).line, (**itLine).clr, c);
			++c[0];
		}
	}
	FOR_EACH(it, m_lsPt)
		helper.Point((**it).pt, (**it).clr, (**it).c);
}

RECT CZoomFigure::GetRect()
{
	return RECT();
}

void CZoomFigure::GetPixInfo(char* buff, int r, int c)
{

}

FigurePt *CZoomFigure::AddPoint(float x, float y, COLORREF clr, const char *c)
{
	auto p = new FigurePt{PointF(x, y), c, clr, true};
	m_lsPt.push_back(p);
	return p;
}

void CZoomFigure::Add(FigurePt* pt)
{
	m_lsPt.push_back(pt);
}

void CZoomFigure::Add(FigureLine* line)
{
	m_lsLine.push_back(line);
}

