#include "IZoom.h"
#include "zoom/impl/ZoomFigure.h"

IBmpMapper::IBmpMapper(int nMulti /*= 1*/) : m_nMulti(nMulti) {}

IBmpMapper::~IBmpMapper() {}

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

IZoom::IZoom(int nCtrlID) : m_nCtrlID(nCtrlID), m_implBmpMapper(nullptr) {}

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
	dynamic_cast<CZoom*>(m_pParentCtrl->m_pParentCtrl)->NotifyEvent(m_nCtrlID * 10000 + nMsgID, pt);
}

void IZoom::NotifyEvent(int nMsgID, const RECT &rc)
{
	dynamic_cast<CZoom*>(m_pParentCtrl->m_pParentCtrl)->NotifyEvent(m_nCtrlID * 10000 + nMsgID, rc);
}

CZoomView::CZoomView()
{
	m_bForceCapture = true;
	m_implZoom = nullptr;
	m_implEdit = new CZoomFigure;
	m_implEdit->ResetRect();
	AddCtrl(m_implEdit, 1);
}

CMouseCapturer* CZoomView::WantCapture(POINT ptParent)
{
	CMouseCapturer* p = this;
	if (ROOT_CTRL(RootCtrl())->KeyDown(KEY_CTRL_L))
		p = CMouseCapturer::WantCapture(ptParent);
	if (p)
		SetCoordinate();
	return p;
}

void CZoomView::Activate(POINT ptWnd)
{
	m_ptDown = WindowToChild(ptWnd);
}

void CZoomView::ActivateMove(POINT ptWnd)
{
	auto pt = WindowToChild(ptWnd);
	m_rcRelLoc.OffsetRect(pt.x - m_ptDown.x, pt.y - m_ptDown.y);
	dynamic_cast<CZoom*>(m_pParentCtrl)->NotifyOffset();
}

void CZoomView::MouseWheel(int zDelta)
{
	m_implZoom->Zoom(zDelta);
	m_implEdit->Zoom(zDelta);
	ResetScroll();
	dynamic_cast<CZoom*>(m_pParentCtrl)->NotifyOffset();
}

char* CZoomView::GetTipString()
{
	return m_strCoordinate;
}

void CZoomView::RBtnDown(POINT pt)
{
	m_implZoom->RBtnDown(pt); //TODO: notify all zoom impl
}

void CZoomView::SetZoomImpl(IZoom *implZoom)
{
	m_implZoom = implZoom;
	m_implZoom->ResetRect();
	AddCtrl(m_implZoom);
}

void CZoomView::SetCoordinate()
{
	float x = m_ptMousePos.x, y = m_ptMousePos.y;
	m_implZoom->Map(y, x);
	m_ptCoordinate.x = x;
	m_ptCoordinate.y = y;

	sprintf(m_strCoordinate, "(%d, %d)", m_ptCoordinate.x, m_ptCoordinate.y);
	m_implZoom->GetPixInfo(&m_strCoordinate[strlen(m_strCoordinate)], m_ptCoordinate.y, m_ptCoordinate.x);
}

void CZoomView::ResetScroll()
{
	float x = m_ptCoordinate.x, y = m_ptCoordinate.y;
	m_implZoom->Revert(y, x);
	auto ptParent = m_pParentCtrl->GetMousePos();
	m_rcRelLoc.MoveToXY(ptParent.x - x, ptParent.y - y);
}

CCoordinate::CCoordinate()
	: m_nBegin(0)
	, m_nMulti(1)
{
}

void CCoordinate::Update(int nScroll, int nMulti)
{
	m_nBegin = -nScroll;
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

void IZoomEventHandler::NotifyEvent(int /*nMsgID*/, const Point<int> &/*pt*/){}
void IZoomEventHandler::NotifyEvent(int /*nMsgID*/, const RECT &/*rc*/){}

CZoom::CZoom(RECT rcRelLoc)
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

void CZoom::SetZoomImpl(IZoom* implZoom)
{
	m_pZoomView->SetZoomImpl(implZoom);
}

void CZoom::NotifyOffset()
{
	auto nMulti = m_pZoomView->m_implZoom->GetMulti();
	m_pCoordinateH->Update(m_pZoomView->m_rcRelLoc.left, nMulti);
	m_pCoordinateV->Update(m_pZoomView->m_rcRelLoc.top, nMulti);
	InvalidateCtrl();
}

void CZoom::NotifyEvent(int nMsgID, const Point<int> &pt)
{
	auto p = dynamic_cast<IZoomEventHandler*>(m_pParentCtrl);
	if (p) p->NotifyEvent(nMsgID, pt);
}

void CZoom::NotifyEvent(int nMsgID, const RECT &rc)
{
	auto p = dynamic_cast<IZoomEventHandler*>(m_pParentCtrl);
	if (p) p->NotifyEvent(nMsgID, rc);
}

