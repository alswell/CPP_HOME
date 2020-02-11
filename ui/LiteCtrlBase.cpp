#include "LiteCtrlBase.h"


ILiteDC::~ILiteDC(){}

CLiteCtrlBase::CLiteCtrlBase(const RECT& rcRelLoc)
	: m_implDC(nullptr)
	, m_rcRelLoc(rcRelLoc)
	, m_ptScroll(0, 0)
	, m_pRootCtrl(nullptr)
	, m_pParentCtrl(nullptr)
	, m_bIsVisible(TRUE)
	, m_nZOrder(0)
{
}

CLiteCtrlBase::CLiteCtrlBase(const RECT& rcRelLoc, CLiteCtrlBase* pParentCtrl)
	: CLiteCtrlBase(rcRelLoc)
{
	m_pRootCtrl = pParentCtrl->m_pRootCtrl;
	m_pParentCtrl = pParentCtrl;
	m_implDC = pParentCtrl->m_implDC;
}

CLiteCtrlBase::~CLiteCtrlBase()
{
}

void CLiteCtrlBase::InitDC(ILiteDC* implDC)
{
	m_implDC = implDC;
}

void CLiteCtrlBase::Draw(const RECT& rcLoc, const RECT& rcViewRgn)
{
}

void CLiteCtrlBase::PreDraw(POINT ptParentPos, RECT rcParentViewRgn)
{
	RECT rcLoc = m_rcRelLoc;
	rcLoc.OffsetRect(ptParentPos);
	RECT rcViewRgn;
	rcViewRgn.IntersectRect(rcLoc, rcParentViewRgn);
	if (rcViewRgn.IsRectEmpty())
		return;
	Draw(rcLoc, rcViewRgn);
	DrawChildren(POINT(rcLoc.left - m_ptScroll.x, rcLoc.top - m_ptScroll.y), rcViewRgn);
}

void CLiteCtrlBase::DrawChildren(POINT ptParentPos /*= CPoint(0, 0)*/, RECT rcParentViewRgn /*= RECT()*/)
{
	for (map<int, vector<CLiteCtrlBase*>>::iterator itCtrl = m_vCtrls.begin(); itCtrl != m_vCtrls.end(); ++itCtrl)
		for (vector<CLiteCtrlBase*>::iterator it = itCtrl->second.begin(); it != itCtrl->second.end(); ++it)
			if ((**it).m_bIsVisible) 
			{
				//OutputDebugString("Draw\n");
				(**it).PreDraw(ptParentPos, rcParentViewRgn);
			}
}

CMouseCapturer* CLiteCtrlBase::WantCapture(POINT ptParent)
{
	m_bIsMouseIn = CG::PtInRect(m_rcRelLoc, ptParent);
	m_ptMousePos = ParentToChild(ptParent);
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

POINT CLiteCtrlBase::GetMousePos()
{
	return m_ptMousePos;
}

RECT CLiteCtrlBase::GetRelLoc()
{
	return m_rcRelLoc;
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

int CLiteCtrlBase::Width()
{
	return m_rcRelLoc.Width();
}

int CLiteCtrlBase::Height()
{
	return m_rcRelLoc.Height();
}
