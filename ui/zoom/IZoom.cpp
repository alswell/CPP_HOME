#include "IZoom.h"

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

void IBmpMapper::Map(int& x, int& y)
{
	float fX = x, fY = y;
	Map(fX, fY);
	x = int(fX);
	y = int(fY);
}

void IBmpMapper::Map(POINT& pt)
{
	Map(pt.x, pt.y);
}

void IBmpMapper::Map(PointF& pt)
{
	Map(pt.x, pt.y);
}

void IBmpMapper::Revert(int& x, int& y)
{
	float fX = x, fY = y;
	Revert(fX, fY);
	x = int(fX);
	y = int(fY);
}

void IBmpMapper::Revert(POINT& pt)
{
	Revert(pt.x, pt.y);
}

void IBmpMapper::Revert(PointF& pt)
{
	Revert(pt.x, pt.y);
}

IZoom::IZoom(int nCtrlID) : m_nCtrlID(nCtrlID), m_implBmpMapper(nullptr) {}

IZoom::~IZoom() {}

void IZoom::SetMapper(IBmpMapper *p)
{
	m_implBmpMapper = p;
}

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
	m_bHyperCtrl = true;
	m_implBmpMapper = nullptr;
}

CMouseCapturer* CZoomView::WantCapture()
{
	CMouseCapturer* p = this;
	if (ROOT_CTRL(RootCtrl())->KeyDown(KEY_CTRL_L))
		p = CMouseCapturer::WantCapture();
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
	InvalidateCtrl();
}

void CZoomView::MouseWheel(int zDelta)
{
	m_implBmpMapper->Zoom(zDelta);
	for (unsigned i = 0; i < m_vZooms.size(); ++i)
		m_vZooms[i]->ResetRect();
	SetScroll(m_ptCoordinate, m_pParentCtrl->GetMousePos());
	InvalidateCtrl();
}

char* CZoomView::GetTipString()
{
	return m_strCoordinate;
}

void CZoomView::RBtnDown(POINT pt)
{
	for (unsigned i = 0; i < m_vZooms.size(); ++i)
		m_vZooms[i]->RBtnDown(pt);
}

void CZoomView::AddZoomImpl(IZoom *implZoom)
{
	m_vZooms.push_back(implZoom);
	implZoom->SetMapper(m_implBmpMapper);
	implZoom->ResetRect();
	AddCtrl(implZoom);
}

void CZoomView::SetCoordinate()
{
	float x = m_ptMousePos.x, y = m_ptMousePos.y;
	m_implBmpMapper->Map(y, x);
	m_ptCoordinate.x = x;
	m_ptCoordinate.y = y;

	sprintf(m_strCoordinate, "(%d, %d)", m_ptCoordinate.x, m_ptCoordinate.y);
	m_vZooms[0]->GetPixInfo(&m_strCoordinate[strlen(m_strCoordinate)], m_ptCoordinate.y, m_ptCoordinate.x);  // m_vZooms[0]
}

void CZoomView::SetScroll(const POINT& ptTarget, const POINT& ptPos)
{
	float x = ptTarget.x, y = ptTarget.y;
	m_implBmpMapper->Revert(y, x);
	m_rcRelLoc.MoveToXY(ptPos.x - x, ptPos.y - y);
}

CCoordinate::CCoordinate()
	: m_nBegin(nullptr)
	, m_implBmpMapper(nullptr)
{
}

void CCoordinateH::Draw(ILiteDC *dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	int beg = *m_nBegin % 100;
	beg < 0 && (beg += 100);
	for (int i = beg+rcLoc.left, x = beg-*m_nBegin; i < rcLoc.right; i+=100, x+=100)
	{
		dc->Line(i, rcLoc.top, i, rcLoc.bottom, CLR_DEFAULT);
		int X = x, Y = 0;
		m_implBmpMapper->Map(Y, X);
		sprintf(m_strCoordinate, "%d", X);
		dc->TextStd(rcViewRgn, RectW(i, rcLoc.top, 40, 20), m_strCoordinate, CLR_DEFAULT);
	}
}

void CCoordinateV::Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	int beg = *m_nBegin % 100;
	beg < 0 && (beg += 100);
	for (int i = beg+rcLoc.top, y = beg-*m_nBegin; i < rcLoc.bottom; i+=100, y+=100)
	{
		dc->Line(rcLoc.left, i, rcLoc.right, i, CLR_DEFAULT);
		int X = 0, Y = y;
		m_implBmpMapper->Map(Y, X);
		sprintf(m_strCoordinate, "%d", Y);
		dc->TextStd(rcViewRgn, RectW(rcLoc.left, i, 40, 20), m_strCoordinate, CLR_DEFAULT);
	}
}

void IZoomEventHandler::NotifyEvent(int /*nMsgID*/, const Point<int> &/*pt*/){}
void IZoomEventHandler::NotifyEvent(int /*nMsgID*/, const RECT &/*rc*/){}

CZoom::CZoom(RECT rcRelLoc)
{
	m_rcRelLoc = rcRelLoc;
	rcRelLoc.MoveToXY(0, 0);
	m_pZoomView = new CZoomView;
	AddCtrl(m_pZoomView);
	m_pCoordinateH = new CCoordinateH;
	m_pCoordinateH->m_nBegin = &m_pZoomView->m_rcRelLoc.left;
	AddCtrl(m_pCoordinateH, RECT(0, rcRelLoc.Height() - 20, rcRelLoc.Width() - 40, rcRelLoc.Height()));
	m_pCoordinateV = new CCoordinateV;
	m_pCoordinateV->m_nBegin = &m_pZoomView->m_rcRelLoc.top;
	AddCtrl(m_pCoordinateV, RECT(rcRelLoc.Width() - 40, 0, rcRelLoc.Width(), rcRelLoc.Height()));
}

void CZoom::SetMapper(IBmpMapper *p)
{
	m_pZoomView->m_implBmpMapper = p;
	m_pCoordinateH->m_implBmpMapper = p;
	m_pCoordinateV->m_implBmpMapper = p;
}

void CZoom::AddZoomImpl(IZoom* implZoom)
{
	m_pZoomView->AddZoomImpl(implZoom);
}

void CZoom::SetScroll(const POINT &ptTarget, const POINT &ptPos)
{
	m_pZoomView->SetScroll(ptTarget, ptPos);
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

