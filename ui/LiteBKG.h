#pragma once
#include "LiteCtrlStatic.h"
#include "LiteBtn.h"


class ILiteContext
{
public:
	virtual ~ILiteContext();
	virtual ILiteDC* GetDC() = 0;
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

	ILiteContext* m_wnd;
	RECT m_rcPaintRgn;
public:
	int m_nWidth;
	int m_nHeight;
	CLiteBKG(int W, int H);
	ILiteDC* Init(int W, int H);
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

class ILiteGlobal
{
public:
	virtual ~ILiteGlobal();
	virtual void Start() = 0;
	virtual ILiteContext* GetContext(CLiteBKG* pBKG) = 0;
	virtual int ScreenWidth() = 0;
	virtual int ScreenHeight() = 0;
};
extern ILiteGlobal* gui;

