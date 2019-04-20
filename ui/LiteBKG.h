#pragma once
#include "LiteCtrlStatic.h"
#include "LiteBtn.h"

class CLiteInterface
{
public:
	virtual ~CLiteInterface();
	virtual void Refresh(RECT& rc) = 0;
	virtual void OnClose() = 0;
};

class CLiteBKG : public CLiteCtrlBase
{
	CONF_THIS_CLS(CLiteBKG);
protected:
	CColorBlock* m_pBKG;
	CTextBlock* m_pTitle;
	CLiteBtn<CLiteCtrlBase>* m_pTip;

	CMouseCapturer* m_pHoverCtrl;
	CMouseCapturer* m_pDownCtrl;
	//static CBJICtrlBase* m_pDownActiveCtrl;
	//static map<CBJICtrlBase*, BOOL> m_pActiveMenu;

	CLiteInterface& m_wnd;
	RECT m_rcPaintRgn;
public:
	int m_nWidth;
	int m_nHeight;
	CLiteBKG(CLiteInterface& wnd, CLiteDCInterface& dcImpl, int W, int H);
	virtual ~CLiteBKG();

private:
	void OnClose();
	virtual void InvalidateCtrl2(RECT& rc);

public:
	void OnPaint();
	void OnMouseMove(POINT pt);
	void OnLBtnDown(POINT pt);
	void OnLBtnUp();
	void OnMouseWhell(int zDelta);
	void OnRBtnDown(POINT pt);
};

