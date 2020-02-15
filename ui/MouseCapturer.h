#pragma once
#include "LiteCtrlBase.h"

class CMouseCapturer : public CLiteCtrlBase
{
public:
	CMouseCapturer(CLiteCtrlBase* pParentCtrl, const RECT& rcRelLoc);
	virtual ~CMouseCapturer();


	virtual CMouseCapturer* WantCapture(POINT ptParent);
	virtual void CaptureMouse(bool bDown) {}
	virtual void ReleaseMouse(bool bDown) {}
	virtual void Activate(POINT ptWnd) {}
	virtual void ActivateMove(POINT ptWnd) {}
	virtual void Inactivate(bool bCapture) {}
	virtual void MouseWheel(int zDelta) {}
	virtual char* GetTipString() { return nullptr; }
	virtual void RBtnDown(POINT pt) {}
};

