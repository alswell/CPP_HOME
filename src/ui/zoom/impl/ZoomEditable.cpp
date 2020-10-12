#include "ZoomEditable.h"


void CZoomEditable::Activate(POINT ptWnd)
{
	m_ptDown = WindowToChild(ptWnd);
	m_implBmpMapper->Map(m_ptDown);
	m_pActiveRect = new FigureRect{CRect<float>(m_ptDown.x, m_ptDown.y, m_ptDown.x, m_ptDown.y), CLR_R, true};
	m_lsRect.push_back(m_pActiveRect);
}

void CZoomEditable::ActivateMove(POINT ptWnd)
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

void CZoomEditable::Inactivate(bool bCapture)
{
	NotifyEvent(ZOOM_NOTIFY_RECT, m_pActiveRect->rc);
}

