#include "MouseCapturer.h"

CMouseCapturer::~CMouseCapturer()
{

}

CMouseCapturer* CMouseCapturer::WantCapture(POINT ptParent)
{
	auto p = CLiteCtrlBase::WantCapture(ptParent);
	return p ? p : this;
//	m_ptMousePos = ParentToChild(ptParent);
//	return this;
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
