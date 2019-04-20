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
