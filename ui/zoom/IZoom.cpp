#include "IZoom.h"

IBmpMapper::IBmpMapper(int nMulti /*= 1*/)
	: m_nMulti(nMulti)
{

}

IBmpMapper::~IBmpMapper()
{

}

void IBmpMapper::Zoom(int iDelta)
{
	if (iDelta > 0)
	{
		if (m_nMulti == -2)
			m_nMulti = 1;
		else
			++m_nMulti;
	}
	else
	{
		if (m_nMulti == 1)
			m_nMulti = -2;
		else
			--m_nMulti;
	}
}

float IBmpMapper::GetZoom()
{
	return m_nMulti > 0 ? m_nMulti : 1.0f / -m_nMulti;
}

IZoom::~IZoom() {}

void IZoom::Zoom(int iDelta)
{
	m_implBmpMapper->Zoom(iDelta);
	ResetRect();
}

void IZoom::Map(float& x, float& y)
{
	m_implBmpMapper->Map(x, y);
}

void IZoom::Revert(float& x, float& y)
{
	m_implBmpMapper->Revert(x, y);
}

int IZoom::GetMulti()
{
	return m_implBmpMapper->m_nMulti;
}

void IZoom::ResetRect()
{
	m_rcRelLoc = GetRect();
}

void IZoom::NotifyEvent(int nMsgID, const Point<int> &pt)
{
	dynamic_cast<CZoom*>(m_pParentCtrl->m_pParentCtrl)->NotifyEvent(nMsgID, pt);
}

void IZoom::NotifyEvent(int nMsgID, const RECT &rc)
{
	dynamic_cast<CZoom*>(m_pParentCtrl->m_pParentCtrl)->NotifyEvent(nMsgID, rc);
}


CZoomView::CZoomView()
{
	m_bDown = false;
	m_implZoom = nullptr;
	m_pRedRect = new CColorBlock(CLR_NONE, CLR_R);
	AddCtrl(m_pRedRect, 0, 0, 0, 0, 1);
	m_pRedRect->ShowCtrl(false);
}

CMouseCapturer* CZoomView::WantCapture(POINT ptParent)
{
	auto p = CMouseCapturer::WantCapture(ptParent);
	if (p)
		SetCoordinate();
	return p;
}

void CZoomView::Activate(POINT ptWnd)
{
	m_bDown = ROOT_CTRL(RootCtrl())->KeyDown(KEY_CTRL_L);
	m_pRedRect->ShowCtrl(true);
	m_ptDown = WindowToChild(ptWnd);

	if (m_bDown)
	{
		auto ptReal = PixCoordinate(m_ptDown);
		m_rcRealRedRect.left = ptReal.x;
		m_rcRealRedRect.top = ptReal.y;
		m_rcRealRedRect.right = ptReal.x;
		m_rcRealRedRect.bottom = ptReal.y;
	}
}

void CZoomView::ActivateMove(POINT ptWnd)
{
	auto pt = WindowToChild(ptWnd);
	if (m_bDown)
	{
		auto ptReal = PixCoordinate(pt);
		m_rcRealRedRect.right = ptReal.x;
		m_rcRealRedRect.bottom = ptReal.y;
		SetRedRect();
		InvalidateCtrl();
	}
	else
	{
		m_ptScroll.x -= pt.x - m_ptDown.x;
		m_ptScroll.y -= pt.y - m_ptDown.y;
		dynamic_cast<CZoom*>(m_pParentCtrl)->NotifyOffset();
	}
}

void CZoomView::Inactivate(bool bCapture)
{
	if (m_bDown)
		dynamic_cast<CZoom*>(m_pParentCtrl)->NotifyRedRect(m_rcRealRedRect);
}

void CZoomView::MouseWheel(int zDelta)
{
	m_implZoom->Zoom(zDelta > 0 ? 1 : -1);
	SetRedRect();
	ResetScroll();
	dynamic_cast<CZoom*>(m_pParentCtrl)->NotifyOffset();
}

char* CZoomView::GetTipString()
{
	return StrCoordinate();
}

void CZoomView::RBtnDown(POINT pt)
{
	dynamic_cast<CZoom*>(m_pParentCtrl)->NotifyRBtnDown();
}

void CZoomView::SetZoomImpl(IZoom *implZoom)
{
	m_implZoom = implZoom;
	m_implZoom->ResetRect();
	AddCtrl(m_implZoom);
}

POINT CZoomView::PixCoordinate(const POINT& pt)
{
	float x = pt.x, y = pt.y;
	m_implZoom->Map(y, x);
	return POINT(int(x), int(y));
}

void CZoomView::SetCoordinate()
{
	m_ptCoordinate = PixCoordinate(m_ptMousePos);
	//m_ptCoordinate.x = pt.x / m_pZoomImg->m_nMulti * m_pZoomImg->m_nMultiD;
	//m_ptCoordinate.y = pt.y / m_pZoomImg->m_nMulti * m_pZoomImg->m_nMultiD;

	sprintf(m_strCoordinate, "(%d, %d)", m_ptCoordinate.x, m_ptCoordinate.y);
	m_implZoom->GetPixInfo(&m_strCoordinate[strlen(m_strCoordinate)], m_ptCoordinate.y, m_ptCoordinate.x);
}

POINT CZoomView::GetCoordinate()
{
	return m_ptCoordinate;
}

char* CZoomView::StrCoordinate()
{
	return m_strCoordinate;
}

void CZoomView::SetRedRect()
{
	float multi = m_implZoom->m_implBmpMapper->GetZoom();
	auto rc = m_rcRealRedRect;
	//Println(multi, rc);
	rc *= multi;
	m_pRedRect->SetRelLoc(rc);
}

void CZoomView::ResetScroll()
{
	//pt.Offset(-m_rcRelLoc.left, -m_rcRelLoc.top);

	float x = m_ptCoordinate.x, y = m_ptCoordinate.y;
	m_implZoom->Revert(y, x);
	auto ptParent = m_pParentCtrl->GetMousePos();
	m_ptScroll.x = int(x) - ptParent.x;
	m_ptScroll.y = int(y) - ptParent.y;

	//m_ptScroll.x = m_ptCoordinate.x * m_pZoomImg->m_nMulti / m_pZoomImg->m_nMultiD - pt.x;
	//m_ptScroll.y = m_ptCoordinate.y * m_pZoomImg->m_nMulti / m_pZoomImg->m_nMultiD - pt.y;
}

CCoordinate::CCoordinate()
	: m_nBegin(0)
	, m_nMulti(1)
{
}

void CCoordinate::Update(int nScroll, int nMulti)
{
	m_nBegin = nScroll;
	m_nMulti = nMulti;
}

void CCoordinateH::Draw(ILiteDC *dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	int tmp = ZOOM_MULTI(100, m_nMulti);
	for (int i = rcLoc.left, x = m_nBegin; i < rcLoc.right; i++, x++)
	{
		if (x % tmp == 0)
		{
			dc->Line(i, rcLoc.top, i, rcLoc.bottom, CLR_DEFAULT);
			sprintf(m_strCoordinate, "%d", ZOOM_DIVID(x, m_nMulti));
			dc->TextStd(rcViewRgn, RectW(i, rcLoc.top, 40, 20), m_strCoordinate, CLR_DEFAULT);
		}
	}
}

void CCoordinateV::Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	int tmp = ZOOM_MULTI(100, m_nMulti);
	for (int i = rcLoc.top, y = m_nBegin; i < rcLoc.bottom; i++, y++)
	{
		if (y % tmp == 0)
		{
			dc->Line(rcLoc.left, i, rcLoc.right, i, CLR_DEFAULT);
			sprintf(m_strCoordinate, "%d", ZOOM_DIVID(y, m_nMulti));
			dc->TextStd(rcViewRgn, RectW(rcLoc.left, i, 40, 20), m_strCoordinate, CLR_DEFAULT);
		}
	}
}

CZoom::CZoom(RECT rcRelLoc, NOTIFY_RBTN_DOWN cb)
	: m_cbNotifyRBtnDown(cb)
	, m_cbNotifyRedRect(nullptr)
	, m_cbEventPt(nullptr)
	, m_cbEventRect(nullptr)
{
	m_rcRelLoc = rcRelLoc;
	rcRelLoc.MoveToXY(0, 0);
	m_pZoomView = new CZoomView;
	AddCtrl(m_pZoomView, rcRelLoc);
	m_pCoordinateH = new CCoordinateH;
	AddCtrl(m_pCoordinateH, RECT(0, rcRelLoc.Height() - 20, rcRelLoc.Width() - 40, rcRelLoc.Height()));
	m_pCoordinateV = new CCoordinateV;
	AddCtrl(m_pCoordinateV, RECT(rcRelLoc.Width() - 40, 0, rcRelLoc.Width(), rcRelLoc.Height()));
}

void CZoom::RegRedRectCB(CZoom::NOTIFY_RED_RECT cb)
{
	m_cbNotifyRedRect = cb;
}

void CZoom::SetZoomImpl(IZoom* implZoom)
{
	m_pZoomView->SetZoomImpl(implZoom);
}

void CZoom::NotifyOffset()
{
	auto nMulti = m_pZoomView->m_implZoom->GetMulti();
	m_pCoordinateH->Update(m_pZoomView->m_ptScroll.x, nMulti);
	m_pCoordinateV->Update(m_pZoomView->m_ptScroll.y, nMulti);
	InvalidateCtrl();
}

void CZoom::NotifyRedRect(const RECT& rc)
{
	if (m_cbNotifyRedRect)
		m_cbNotifyRedRect(m_pParentCtrl, rc);
}

void CZoom::NotifyRBtnDown()
{
	if (m_cbNotifyRBtnDown)
		m_cbNotifyRBtnDown(m_pParentCtrl, Point<int>(GetCoordinate()));
}

POINT CZoom::GetCoordinate()
{
	return m_pZoomView->m_ptCoordinate;
}

void CZoom::NotifyEvent(int nMsgID, const Point<int> &pt)
{
	if (m_cbEventPt)
		m_cbEventPt(m_pParentCtrl, nMsgID, pt);
}

void CZoom::NotifyEvent(int nMsgID, const RECT &rc)
{
	if (m_cbEventRect)
		m_cbEventRect(m_pParentCtrl, nMsgID, rc);
}
