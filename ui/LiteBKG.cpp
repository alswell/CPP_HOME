#include "LiteBKG.h"


ILiteGlobal* gui;

ILiteContext::~ILiteContext()
{
}

void ILiteContext::SetBkgAndInit(CLiteBKG* pBKG)
{
	m_pBKG = pBKG;
	Init();
}

void ILiteContext::OnPaint()
{
	m_pBKG->OnPaint();
}

void ILiteContext::OnMouseMove(const POINT& pt)
{
	m_pBKG->OnMouseMove(pt);
}

void ILiteContext::OnLBtnDown(const POINT& pt)
{
	m_pBKG->OnLBtnDown(pt);
}

void ILiteContext::OnLBtnUp()
{
	m_pBKG->OnLBtnUp();
}

void ILiteContext::OnMouseWheel(int zDelta)
{
	m_pBKG->OnMouseWheel(zDelta);
}

void ILiteContext::OnRBtnDown(const POINT& pt)
{
	m_pBKG->OnRBtnDown(pt);
}

ILiteGlobal::~ILiteGlobal()
{

}

ILiteContext* ILiteGlobal::GetContext(CLiteBKG* pBKG)
{
	auto p = CreateContext();
	p->SetBkgAndInit(pBKG);
	return p;
}

CLiteBKG::CLiteBKG(int W, int H)
	: CLiteCtrlBase(RECT(0, 0, W, H))
	, m_rcPaintRgn(0, 0, W, H)
{
	m_implContext = gui->GetContext(this);
	InitDC(m_implContext->GetDC());

	m_pHoverCtrl = nullptr;
	m_pDownCtrl = nullptr;

	m_pTip = ADD_TIP(0, 0, 120, 60);
	m_pTip->ShowCtrl(FALSE);

	m_pBKG = ADD_BLOCK(0, 0, W, H, RGBH(1EA084), RGBH(00FF00));
//	ADD_BTN(W - 20, 0, 20, 20, "X", StdBtn, OnClose);
}

CLiteBKG::~CLiteBKG()
{
}

void CLiteBKG::OnClose()
{
	m_implContext->OnClose();
}

void CLiteBKG::InvalidateCtrl2(RECT& rc)
{
	m_rcPaintRgn.UnionRect(RECT(m_rcPaintRgn), rc);
	m_implContext->Refresh(rc);
}

void CLiteBKG::OnPaint()
{
	if (m_rcPaintRgn.IsRectEmpty())
		return;
	DrawChildren(POINT(0, 0), m_rcRelLoc);
	m_rcPaintRgn.SetRectEmpty();
}

void CLiteBKG::OnMouseMove(const POINT& pt)
{
	m_pTip->MoveTo(pt.x + 20, pt.y + 20);
	if (m_pDownCtrl)
		m_pDownCtrl->ActivateMove(pt);
	CMouseCapturer* pHandler = WantCapture(pt);
	if (m_pHoverCtrl != pHandler)
	{
		if (m_pHoverCtrl)
			m_pHoverCtrl->ReleaseMouse(m_pDownCtrl && m_pDownCtrl == m_pHoverCtrl/*pHandler*/);
		m_pHoverCtrl = pHandler;
		if ((!m_pDownCtrl || m_pDownCtrl == m_pHoverCtrl) && m_pHoverCtrl)
			m_pHoverCtrl->CaptureMouse(m_pDownCtrl && m_pDownCtrl == m_pHoverCtrl);
	}

	bool bShowTip = false;
	if (m_pHoverCtrl)
	{
		char* str = nullptr;
		if (!(m_pDownCtrl && m_pDownCtrl != m_pHoverCtrl))
			str = m_pHoverCtrl->GetTipString();
		if (str)
		{
			bShowTip = true;
			m_pTip->SetText(str);
		}
	}
	m_pTip->ShowCtrl(bShowTip);
}

void CLiteBKG::OnLBtnDown(const POINT& pt)
{
	if (m_pHoverCtrl)
	{
		m_pDownCtrl = m_pHoverCtrl;
		m_pDownCtrl->Activate(pt);
	}
}

void CLiteBKG::OnLBtnUp()
{
	if (m_pDownCtrl)
	{
		if (m_pDownCtrl == m_pHoverCtrl)
		{
			m_pDownCtrl->Inactivate(true);
		}
		else
		{
			m_pDownCtrl->Inactivate(false);
			if (m_pHoverCtrl)
				m_pHoverCtrl->CaptureMouse(false);
		}
		m_pDownCtrl = nullptr;
	}
}

void CLiteBKG::OnMouseWheel(int zDelta)
{
	if (m_pHoverCtrl && !m_pDownCtrl)
	{
		m_pHoverCtrl->MouseWheel(zDelta);
	}
}

void CLiteBKG::OnRBtnDown(const POINT& pt)
{
	m_pHoverCtrl->RBtnDown(pt);
}

