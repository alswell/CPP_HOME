#include "LiteCtrlBase.h"


CLiteDCInterface::~CLiteDCInterface(){}

CLiteCtrlBase::CLiteCtrlBase(RECT rcRelLoc, CLiteDCInterface & dcImpl)
	: m_bAlpha(FALSE)
	, m_pOldRect(NULL)
	, m_bIsVisible(TRUE)
	, m_ptScroll(0, 0)
	, m_rcRelLoc(rcRelLoc)
	, m_pParentCtrl(NULL)
	, m_nZOrder(0)
	, m_dcImpl(dcImpl)
{
	m_szBmp.cx = m_rcRelLoc.Width();
	m_szBmp.cy = m_rcRelLoc.Height();
}

CLiteCtrlBase::CLiteCtrlBase(RECT rcRelLoc, CLiteCtrlBase* pParentCtrl)
	: CLiteCtrlBase(rcRelLoc, pParentCtrl->m_dcImpl)
{
	m_pParentCtrl = pParentCtrl;
}

CLiteCtrlBase::~CLiteCtrlBase()
{
}

void CLiteCtrlBase::Draw(POINT ptOffset, RECT rcParentRgn)
{
	RECT rcDraw = m_rcRelLoc;
	rcDraw.OffsetRect(ptOffset);
	RECT rcRgn;
	rcRgn.IntersectRect(rcDraw, rcParentRgn);
	rcDraw.OffsetRect(-m_ptScroll);
	DrawChildren(POINT(rcDraw.left, rcDraw.top), rcRgn);
}

void CLiteCtrlBase::DrawChildren(POINT ptOffset /*= CPoint(0, 0)*/, RECT rcRgn /*= RECT()*/)
{
	for (map<int, vector<CLiteCtrlBase*>>::iterator itCtrl = m_vCtrls.begin(); itCtrl != m_vCtrls.end(); ++itCtrl)
		for (vector<CLiteCtrlBase*>::iterator it = itCtrl->second.begin(); it != itCtrl->second.end(); ++it)
			if ((**it).m_bIsVisible) 
			{
				//OutputDebugString("Draw\n");
				(**it).Draw(ptOffset, rcRgn);
			}
}

CMouseCapturer* CLiteCtrlBase::WantCapture(POINT pt)
{
	m_bIsMouseIn = CG::PtInRect(m_rcRelLoc, pt);
	m_ptMousePos = ParentToChild(pt);
	for (map<int, vector<CLiteCtrlBase*>>::iterator itCtrl = m_vCtrls.begin(); itCtrl != m_vCtrls.end(); ++itCtrl)
		for (vector<CLiteCtrlBase*>::iterator it = itCtrl->second.begin(); it != itCtrl->second.end(); ++it)
			if ((**it).m_bIsVisible)
			{
				//OutputDebugString("MouseMove\n");
				CMouseCapturer* pHandler = (**it).WantCapture(m_ptMousePos);
				if (pHandler)
					return pHandler;
			}
	return nullptr;
}

void CLiteCtrlBase::InvalidateCtrl()
{
	if (!m_bIsVisible)
		return;
	RECT rc(m_rcRelLoc);
	if (m_pParentCtrl)
		m_pParentCtrl->InvalidateCtrl2(rc);
	else
		this->InvalidateCtrl2(rc);
}

void CLiteCtrlBase::InvalidateCtrl2(RECT & rc)
{
	rc.OffsetRect(m_rcRelLoc.left, m_rcRelLoc.top);
	rc.OffsetRect(-m_ptScroll);
	if (m_pParentCtrl)
		m_pParentCtrl->InvalidateCtrl2(rc);
}

void CLiteCtrlBase::ShowCtrl(BOOL bShow)
{
	if (m_bIsVisible == bShow)
		return;

	m_bIsVisible = bShow;
	InvalidateCtrl();
}

POINT CLiteCtrlBase::ParentToChild(POINT pt)
{
	pt.Offset(-m_rcRelLoc.left + m_ptScroll.x, -m_rcRelLoc.top + m_ptScroll.y);
	return pt;
}

CLiteCtrlBase* CLiteCtrlBase::AddCtrl(CLiteCtrlBase * pCtrl, int nZOrder/* = 0*/)
{
	m_vCtrls[nZOrder].push_back(pCtrl);
	return pCtrl;
}

RECT CLiteCtrlBase::GetDrawRect()
{
	RECT rc = m_rcRelLoc;
	rc.MoveToXY(m_ptScroll.x, m_ptScroll.y);
	return rc;
}

void CLiteCtrlBase::MoveToX(int x)
{
	if (m_rcRelLoc.left == x)
		return;
	InvalidateCtrl();
	m_rcRelLoc.MoveToX(x);
	InvalidateCtrl();
}

void CLiteCtrlBase::MoveToY(int y)
{
	if (m_rcRelLoc.top == y)
		return;
	InvalidateCtrl();
	m_rcRelLoc.MoveToY(y);
	InvalidateCtrl();
}

void CLiteCtrlBase::MoveTo(int x, int y)
{
	if (m_rcRelLoc.left == x && m_rcRelLoc.top == y)
		return;
	InvalidateCtrl();
	m_rcRelLoc.MoveToXY(x, y);
	InvalidateCtrl();
}

void CLiteCtrlBase::Move(int x, int y)
{
	if (!x && !y)
		return;
	InvalidateCtrl();
	m_rcRelLoc.OffsetRect(x, y);
	InvalidateCtrl();
}