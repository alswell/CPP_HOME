#pragma once
#include "LiteCtrlBase.h"

class CMouseCapturer : public CLiteCtrlBase
{
public:
	CMouseCapturer(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc);
	virtual ~CMouseCapturer();


	virtual CMouseCapturer* WantCapture(POINT pt);
	virtual void CaptureMouse() {}
	virtual void ReleaseMouse(bool bDown) {}
	virtual void Activate(POINT pt) {}
	virtual void ActivateMove(POINT pt) {}
	virtual void Inactivate(bool bCapture) {}
	virtual void MouseWhell(int zDelta) {}
	virtual char* GetTipString() { return NULL; }
	virtual void RBtnDown(POINT pt) {}
};
