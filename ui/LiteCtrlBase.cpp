#include "LiteCtrlBase.h"


ILiteDC::~ILiteDC(){}

CLiteCtrlBase::CLiteCtrlBase()
	: m_bForceCapture(false)
	, m_pParentCtrl(nullptr)
	, m_bIsVisible(TRUE)
	, m_nZOrder(0)
{
	m_strDebugName = nullptr;
}

CLiteCtrlBase::~CLiteCtrlBase()
{
}

void CLiteCtrlBase::Draw(ILiteDC* /*dc*/, const RECT& /*rcLoc*/, const RECT& /*rcViewRgn*/)
{
}

//#define DEBUG_DRAW
void CLiteCtrlBase::WrapDraw(ILiteDC *dc, POINT ptParentPos, RECT rcParentViewRgn)
{
	RECT rcLoc = m_rcRelLoc;
	rcLoc.OffsetRect(ptParentPos);
	RECT rcViewRgn;
	rcViewRgn.IntersectRect(rcLoc, rcParentViewRgn);
	if (rcViewRgn.IsRectEmpty())
		return;
	Draw(dc, rcLoc, rcViewRgn);
	#ifdef DEBUG_DRAW
	if (m_strDebugName)
		printf("DRAW: %s[%d, %d]%d*%d\n", m_strDebugName, rcLoc.left, rcLoc.top, rcLoc.Width(), rcLoc.Height());
	#endif
	DrawChildren(dc, POINT(rcLoc.left, rcLoc.top), rcViewRgn);
}

void CLiteCtrlBase::DrawChildren(ILiteDC *dc, POINT ptParentPos /*= CPoint(0, 0)*/, RECT rcParentViewRgn /*= RECT()*/)
{
	for (map<int, vector<CLiteCtrlBase*>>::iterator itCtrl = m_vCtrls.begin(); itCtrl != m_vCtrls.end(); ++itCtrl)
		for (vector<CLiteCtrlBase*>::iterator it = itCtrl->second.begin(); it != itCtrl->second.end(); ++it)
			if ((**it).m_bIsVisible) 
				(**it).WrapDraw(dc, ptParentPos, rcParentViewRgn);
}

CMouseCapturer* CLiteCtrlBase::WantCapture()
{
	for (map<int, vector<CLiteCtrlBase*>>::iterator itCtrl = m_vCtrls.begin(); itCtrl != m_vCtrls.end(); ++itCtrl)
		for (vector<CLiteCtrlBase*>::iterator it = itCtrl->second.begin(); it != itCtrl->second.end(); ++it)
			if ((**it).m_bIsVisible && ((**it).m_bForceCapture || CG::PtInRect((**it).m_rcRelLoc, m_ptMousePos)))
			{
				(**it).m_ptMousePos = (**it).ParentToChild(m_ptMousePos);
				CMouseCapturer* pHandler = (**it).WantCapture();
				if (pHandler)
					return pHandler;
			}
	return nullptr;
}

void CLiteCtrlBase::LocInParent(RECT& rcChild)
{
	rcChild.OffsetRect(m_rcRelLoc.left, m_rcRelLoc.top);
	if (m_pParentCtrl)
		m_pParentCtrl->LocInParent(rcChild);
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
	if (m_pParentCtrl)
		m_pParentCtrl->InvalidateCtrl2(rc);
}

CLiteCtrlBase *CLiteCtrlBase::RootCtrl()
{
	if (m_pParentCtrl)
		return m_pParentCtrl->RootCtrl();
	return this;
}

void CLiteCtrlBase::ShowCtrl(BOOL bShow)
{
	if (m_bIsVisible == bShow)
		return;

	m_bIsVisible = bShow;
	InvalidateCtrl();
}

RECT CLiteCtrlBase::AbsLoc()
{
	RECT rc(m_rcRelLoc);
	m_pParentCtrl->LocInParent(rc);
	return rc;
}

POINT CLiteCtrlBase::WindowToChild(POINT pt)
{
	auto rc = AbsLoc();
	pt.Offset(-rc.left, -rc.top);
	return pt;
}

POINT CLiteCtrlBase::ParentToChild(POINT pt)
{
	pt.Offset(-m_rcRelLoc.left, -m_rcRelLoc.top);
	return pt;
}

CLiteCtrlBase* CLiteCtrlBase::AddCtrl(CLiteCtrlBase * pCtrl, int nZOrder/* = 0*/)
{
	pCtrl->m_pParentCtrl = this;
	m_vCtrls[nZOrder].push_back(pCtrl);
	return pCtrl;
}

CLiteCtrlBase *CLiteCtrlBase::AddCtrl(CLiteCtrlBase *pCtrl, int left, int top, int width, int height, int nZOrder)
{
	pCtrl->SetRelLoc(RECT(left, top, left+width, top+height));
	return AddCtrl(pCtrl, nZOrder);
}

CLiteCtrlBase *CLiteCtrlBase::AddCtrl(CLiteCtrlBase *pCtrl, const RECT& rc, int nZOrder)
{
	pCtrl->SetRelLoc(rc);
	return AddCtrl(pCtrl, nZOrder);
}

POINT CLiteCtrlBase::GetMousePos()
{
	return m_ptMousePos;
}

RECT CLiteCtrlBase::SetRelLoc(const RECT& rc)
{
	auto old = m_rcRelLoc;
	m_rcRelLoc = rc;
	return old;
}

RECT CLiteCtrlBase::GetRelLoc()
{
	return m_rcRelLoc;
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
