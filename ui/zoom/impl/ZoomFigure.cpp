#include "ZoomFigure.h"

CZoomFigure::CZoomFigure()
{
	m_bHyperCtrl = true;
}

void CZoomFigure::Draw(ILiteDC *dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	CPainHelper helper(this->m_implBmpMapper, dc, rcLoc, rcViewRgn);
	char c = 'A';
	FOR_EACH(itLine, m_lsLine)
	{
		if ((**itLine).c != 0)
		{
			if ((**itLine).c == '$')
			{
				helper.Plot((**itLine).line, (**itLine).clr, c);
				++c;
			}
			else
			{
				helper.Plot((**itLine).line, (**itLine).clr, (**itLine).c);
			}
		}
		else
		{
			helper.Plot((**itLine).line, (**itLine).clr, 0);
		}
	}
	FOR_EACH(it, m_lsPt)
		helper.Point((**it).pt, (**it).clr, (**it).c);
	FOR_EACH(it, m_lsRect)
		helper.Rect((**it).rc, (**it).clr, (**it).clr_fill);
}

RECT CZoomFigure::GetRect()
{
	return RECT();
}

void CZoomFigure::GetPixInfo(char* buff, int r, int c)
{

}

FigurePt *CZoomFigure::AddPoint(float x, float y, COLORREF clr, char c)
{
	auto p = new FigurePt{PointF(x, y), c, clr, true};
	m_lsPt.push_back(p);
	return p;
}

FigureRect *CZoomFigure::AddRect(float l, float t, float W, float H, COLORREF clr)
{
	auto p = new FigureRect{CRect<float>(l, t, l+W, t+H), CLR_NONE, clr, true};
	m_lsRect.push_back(p);
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

