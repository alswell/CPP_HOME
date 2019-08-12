#include "MouseCapturer.h"

CMouseCapturer::CMouseCapturer(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc)
	: CLiteCtrlBase(rcRelLoc, pParentCtrl)
{
}

CMouseCapturer::~CMouseCapturer()
{

}

CMouseCapturer* CMouseCapturer::WantCapture(POINT pt)
{
	m_bIsMouseIn = CG::PtInRect(m_rcRelLoc, pt);
	m_ptMousePos = ParentToChild(pt);
	if (m_bIsMouseIn)
		return this;
	return nullptr;
}

CapturerMove::CapturerMove(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc)
	: CMouseCapturer(pParentCtrl, rcRelLoc)
{

}

void CapturerMove::Activate(POINT pt)
{
	m_ptTmpPos=pt;
}

void CapturerMove::ActivateMove(POINT pt)
{
	int x = pt.x - m_ptTmpPos.x;
	int y = pt.y - m_ptTmpPos.y;
	m_ptTmpPos = pt;
	NotifyOffset(x, y);
}
