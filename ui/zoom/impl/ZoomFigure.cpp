#include "ZoomFigure.h"

CZoomFigure::CZoomFigure()
{
	m_bHyperCtrl = true;
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
	FOR_EACH(it, m_lsRect)
		helper.Rect((**it).rc, (**it).clr);
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

FigureRect *CZoomFigure::AddRect(float l, float t, float W, float H, COLORREF clr)
{
	auto p = new FigureRect{CRect<float>(l, t, l+W, t+H), clr, true};
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

void CZoomFigure::Activate(POINT ptWnd)
{
	m_ptDown = WindowToChild(ptWnd);
	m_implBmpMapper->Map(m_ptDown.y, m_ptDown.x);
	m_pActiveRect = new FigureRect{CRect<float>(m_ptDown.x, m_ptDown.y, m_ptDown.x, m_ptDown.y), CLR_R, true};
	m_lsRect.push_back(m_pActiveRect);
}

void CZoomFigure::ActivateMove(POINT ptWnd)
{
	auto pt = WindowToChild(ptWnd);
	float x = pt.x, y = pt.y;
	m_implBmpMapper->Map(y, x);
	if (x > m_ptDown.x)
	{
		m_pActiveRect->rc.right = x;
		m_pActiveRect->rc.left = m_ptDown.x;
	}
	else
	{
		m_pActiveRect->rc.left = x;
		m_pActiveRect->rc.right = m_ptDown.x;
	}
	if (y > m_ptDown.y)
	{
		m_pActiveRect->rc.bottom = y;
		m_pActiveRect->rc.top = m_ptDown.y;
	}
	else
	{
		m_pActiveRect->rc.top = y;
		m_pActiveRect->rc.bottom = m_ptDown.y;
	}
	InvalidateCtrl();
}

void CZoomFigure::Inactivate(bool bCapture)
{
	NotifyEvent(ZOOM_NOTIFY_RECT, m_pActiveRect->rc);
}

CPainHelper::CPainHelper(CZoomFigure* pZoom, ILiteDC* implDC, const RECT& rcLoc, const RECT& rcViewRgn)
	: m_pZoom(pZoom)
	, m_implDC(implDC)
	, m_rcLoc(rcLoc)
	, m_rcViewRgn(rcViewRgn)
{
}

void CPainHelper::PtRevert(float &x, float &y)
{
	m_pZoom->Revert(y, x);
	y += m_rcLoc.top;
	x += m_rcLoc.left;
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

void CPainHelper::Rect(CRect<float> rc, COLORREF clr)
{
	PtRevert(rc.left, rc.top);
	PtRevert(rc.right, rc.bottom);
	m_implDC->Rectangle(m_rcViewRgn, RECT(rc), clr, CLR_NONE);
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
