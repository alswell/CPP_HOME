#include "IZoom.h"

IBmpMapper::IBmpMapper(int nMulti /*= 1*/, int nMultiD /*= 1*/)
	: m_nMulti(nMulti)
	, m_nMultiD(nMultiD)
{

}

IBmpMapper::~IBmpMapper()
{

}

void IBmpMapper::Zoom(int iDelta)
{
	if (iDelta > 0)
	{
		if (m_nMultiD > 1)
			m_nMultiD--;
		else
			m_nMulti++;
	}
	else
	{
		if (m_nMulti > 1)
			m_nMulti--;
		else
			m_nMultiD++;
	}
}

float IBmpMapper::GetZoom()
{
	return float(m_nMulti) / float(m_nMultiD);
}

IZoom::~IZoom() {}

void IZoom::Zoom(int iDelta)
{
	m_implBmpMapper->Zoom(iDelta);
}

void IZoom::Map(float& x, float& y)
{
	m_implBmpMapper->Map(x, y);
}

void IZoom::Revert(float& x, float& y)
{
	m_implBmpMapper->Revert(x, y);
}

void IZoom::GetMulti(int& nMulti, int& nMultiD)
{
	nMulti = m_implBmpMapper->m_nMulti;
	nMultiD = m_implBmpMapper->m_nMultiD;
}

CZoomImg::CZoomImg()
{
	m_implZoom = nullptr;
}

void CZoomImg::SetZoomImpl(IZoom* implZoom)
{
	m_implZoom = implZoom;
	ResetRect();
}

void CZoomImg::Draw(ILiteDC *dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	if (!m_implZoom)
		return;
	m_implZoom->Draw(dc, rcLoc, rcViewRgn);

	//DRAW_INIT(rcDraw, m_rcRelLoc, ptOffset, rcRgn, rcParentRgn);
	//m_dcImpl.ZoomImg(rcRgn, rcDraw, m_pImg, m_nImgW, m_nImgH, *m_mapper);  POINT ptParentPos, RECT rcParentViewRgn
}

void CZoomImg::Zoom(int iDelta)
{
	m_implZoom->Zoom(iDelta);
	ResetRect();
}

void CZoomImg::ResetRect()
{
	m_rcRelLoc = m_implZoom->GetRect();
}

CZoomView::CZoomView()
{
	m_bDown = false;
	m_pZoomImg = new CZoomImg;
	AddCtrl(m_pZoomImg);
	m_pRedRect = ADD_BLOCK(0, 0, 0, 0, CLR_NONE, CLR_R);
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
	m_pZoomImg->Zoom(zDelta > 0 ? 1 : -1);
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

CZoomImg& CZoomView::GetZoomImg()
{
	return *m_pZoomImg;
}

POINT CZoomView::PixCoordinate(const POINT& pt)
{
	float x = pt.x, y = pt.y;
	m_pZoomImg->m_implZoom->Map(y, x);
	return POINT(int(x), int(y));
}

void CZoomView::SetCoordinate()
{
	m_ptCoordinate = PixCoordinate(m_ptMousePos);
	//m_ptCoordinate.x = pt.x / m_pZoomImg->m_nMulti * m_pZoomImg->m_nMultiD;
	//m_ptCoordinate.y = pt.y / m_pZoomImg->m_nMulti * m_pZoomImg->m_nMultiD;

	sprintf(m_strCoordinate, "(%d, %d)", m_ptCoordinate.x, m_ptCoordinate.y);
	m_pZoomImg->m_implZoom->GetPixInfo(&m_strCoordinate[strlen(m_strCoordinate)], m_ptCoordinate.y, m_ptCoordinate.x);
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
	float multi = m_pZoomImg->m_implZoom->m_implBmpMapper->GetZoom();
	auto rc = m_rcRealRedRect;
	//Println(multi, rc);
	rc *= multi;
	m_pRedRect->SetRelLoc(rc);
}

void CZoomView::ResetScroll()
{
	//pt.Offset(-m_rcRelLoc.left, -m_rcRelLoc.top);

	float x = m_ptCoordinate.x, y = m_ptCoordinate.y;
	m_pZoomImg->m_implZoom->Revert(y, x);
	auto ptParent = m_pParentCtrl->GetMousePos();
	m_ptScroll.x = int(x) - ptParent.x;
	m_ptScroll.y = int(y) - ptParent.y;

	//m_ptScroll.x = m_ptCoordinate.x * m_pZoomImg->m_nMulti / m_pZoomImg->m_nMultiD - pt.x;
	//m_ptScroll.y = m_ptCoordinate.y * m_pZoomImg->m_nMulti / m_pZoomImg->m_nMultiD - pt.y;
}

CCoordinate::CCoordinate()
	: m_nBegin(0)
	, m_nMulti(1)
	, m_nMultiD(1)
{
}

void CCoordinate::Update(int nScroll, int nMulti, int nMultiD)
{
	m_nBegin = nScroll;
	m_nMulti = nMulti;
	m_nMultiD = nMultiD;
}

int CCoordinate::GetBeginValue()
{
	return m_nBegin / m_nMulti * m_nMultiD;
}

int CCoordinate::Multi()
{
	return m_nMulti;
}

int CCoordinate::MultiD()
{
	return m_nMultiD;
}

void CCoordinateH::Draw(ILiteDC *dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	for (int i = rcLoc.left, x = m_nBegin; i < rcLoc.right; i++, x++)
	{
		if (x % (100 * m_nMulti / m_nMultiD) == 0)
		{
			dc->Line(i, rcLoc.top, i, rcLoc.bottom, CLR_DEFAULT);
			sprintf(m_strCoordinate, "%d", x / m_nMulti * m_nMultiD);
			dc->TextStd(rcViewRgn, RectW(i, rcLoc.top, 40, 20), m_strCoordinate, CLR_DEFAULT);
		}
	}
}

void CCoordinateV::Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	for (int i = rcLoc.top, y = m_nBegin; i < rcLoc.bottom; i++, y++)
	{
		if (y % (100 * m_nMulti / m_nMultiD) == 0)
		{
			dc->Line(rcLoc.left, i, rcLoc.right, i, CLR_DEFAULT);
			sprintf(m_strCoordinate, "%d", y / m_nMulti * m_nMultiD);
			dc->TextStd(rcViewRgn, RectW(rcLoc.left, i, 40, 20), m_strCoordinate, CLR_DEFAULT);
		}
	}
}

CZoom::CZoom(RECT rcRelLoc, NOTIFY_RBTN_DOWN cb)
	: m_cbNotifyRBtnDown(cb)
	, m_cbNotifyRedRect(nullptr)
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
	m_pZoomView->m_pZoomImg->SetZoomImpl(implZoom);
}

void CZoom::NotifyOffset()
{
	int nMulti, nMultiD;
	m_pZoomView->m_pZoomImg->m_implZoom->GetMulti(nMulti, nMultiD);
	m_pCoordinateH->Update(m_pZoomView->m_ptScroll.x, nMulti, nMultiD);
	m_pCoordinateV->Update(m_pZoomView->m_ptScroll.y, nMulti, nMultiD);
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
