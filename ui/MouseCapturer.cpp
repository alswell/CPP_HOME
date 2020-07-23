#include "MouseCapturer.h"

CMouseCapturer::~CMouseCapturer()
{

}

CMouseCapturer* CMouseCapturer::WantCapture(POINT ptParent)
{
	m_bIsMouseIn = CG::PtInRect(m_rcRelLoc, ptParent);
	m_ptMousePos = ParentToChild(ptParent);
	if (m_bIsMouseIn)
		return this;
	return nullptr;
}

//CapturerMove::CapturerMove(CLiteCtrlBase* pParentCtrl, const RECT& rcRelLoc)
//	: CMouseCapturer(pParentCtrl, rcRelLoc)
//{

//}

//void CapturerMove::Activate(POINT ptParent)
//{
//	m_ptTmpPos = ptParent;
//}

//void CapturerMove::ActivateMove(POINT ptParent)
//{
//	int x = ptParent.x - m_ptTmpPos.x;
//	int y = ptParent.y - m_ptTmpPos.y;
//	m_ptTmpPos = ptParent;
//	NotifyOffset(x, y);
//}
