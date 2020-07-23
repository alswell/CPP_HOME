#include "ZoomFigure.h"

CZoomFigure::CZoomFigure()
{
	m_implBmpMapper = new CStdMapper;
}

void CZoomFigure::Draw(ILiteDC *dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	CPainHelper helper(this, dc, rcLoc, rcViewRgn);
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
	return RECT(0, 0, 1000 * m_implBmpMapper->m_nMulti / m_implBmpMapper->m_nMultiD, 1000 * m_implBmpMapper->m_nMulti / m_implBmpMapper->m_nMultiD);
}

void CZoomFigure::GetPixInfo(char* buff, int r, int c)
{

}

FigurePt *CZoomFigure::Add(float x, float y, COLORREF clr, const char *c)
{
	auto p = new FigurePt{PointF(x, y), clr, c, true};
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

CPainHelper::CPainHelper(CZoomFigure* pZoom, ILiteDC* implDC, const RECT& rcLoc, const RECT& rcViewRgn)
		: m_pZoom(pZoom)
		, m_implDC(implDC)
		, m_rcLoc(rcLoc)
		, m_rcViewRgn(rcViewRgn)
{
}

void CPainHelper::PtRevert(PointF& pt)
{
	m_pZoom->Revert(pt.y, pt.x);
	pt.y += m_rcLoc.top;
	pt.x += m_rcLoc.left;
}

void CPainHelper::Point(PointF pt0, COLORREF clr, const char* c)
{
	PtRevert(pt0);
	POINT pt(pt0);
	if (m_rcViewRgn.IsPtInside(pt))
	{
		m_implDC->TextStd(RECT(), RectW(pt.x - 14, pt.y - 6, 30, 10), c, clr);
//		m_implDC->TextStd(RECT(), RectW(pt.x - 14, pt.y - 6, 30, 10), "+", clr);
//		m_implDC->TextStd(RECT(), RectW(pt.x - 14, pt.y - 4, 30, 10), "*", clr);
//		m_implDC->TextStd(RECT(), RectW(pt.x - 14, pt.y - 4, 30, 10), "123", clr);
//		m_implDC->Point(pt.x, pt.y, RGBH(00ff00));
	}
}

void CPainHelper::Line(PointF pt1, PointF pt2, COLORREF clr)
{
	PtRevert(pt1);
	PtRevert(pt2);
	m_implDC->Line(m_rcViewRgn, int(pt1.x), int(pt1.y), int(pt2.x), int(pt2.y), clr);
	//m_dcImpl.TextStd(RECT(), RectW(pt1.x - 4, pt1.y - 4, 10, 10), "*", RGBH(0000ff));
	//m_dcImpl.TextStd(RECT(), RectW(pt2.x - 4, pt2.y - 4, 10, 10), "*", RGBH(0000ff));
}

void CPainHelper::Plot(const list<PointF>& ls, COLORREF clr, const char* c)
{
	auto it = ls.begin();
	auto it0 = it; ++it;
	for (; it != ls.end(); ++it0, ++it)
		Line(*it0, *it, clr);
	if (c != nullptr)
	{
		if (c[0] == '!')
		{
			char buff[8];
			int i = 0;
			FOR_EACH(it, ls)
			{
				sprintf(buff, "%d", i);
				Point(*it, clr, buff);
				++i;
			}
		}
		else
		{
			FOR_EACH(it, ls)
			Point(*it, clr, c);
		}
	}
}
