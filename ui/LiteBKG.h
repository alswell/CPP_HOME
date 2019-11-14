#pragma once
#include "LiteCtrlStatic.h"
#include "LiteBtn.h"


class CLiteBKG;
class ILiteContext
{
protected:
	CLiteBKG* m_pBKG;
public:
	virtual ~ILiteContext();
	virtual void Init() = 0;
	virtual ILiteDC* GetDC() = 0;
	virtual void Refresh(RECT& rc) = 0;
	virtual void OnClose() = 0;

	void SetBkgAndInit(CLiteBKG* pBKG);
	void OnPaint();
	void OnMouseMove(const POINT& pt);
	void OnLBtnDown(const POINT& pt);
	void OnLBtnUp();
	void OnMouseWheel(int zDelta);
	void OnRBtnDown(const POINT& pt);
};
class ILiteGlobal
{
public:
	virtual ~ILiteGlobal();
	virtual ILiteContext* CreateContext() = 0;
	virtual void Start() = 0;
	virtual int ScreenWidth() = 0;
	virtual int ScreenHeight() = 0;
	ILiteContext* GetContext(CLiteBKG* pBKG);
};
extern ILiteGlobal* gui;

class CLiteBKG : public CLiteCtrlBase
{
protected:
	CColorBlock* m_pBKG;
	CLiteMagicBox* m_pTip;

	CMouseCapturer* m_pHoverCtrl;
	CMouseCapturer* m_pDownCtrl;
	//static CBJICtrlBase* m_pDownActiveCtrl;
	//static map<CBJICtrlBase*, BOOL> m_pActiveMenu;

	ILiteContext* m_implContext;
	RECT m_rcPaintRgn;
public:
	CLiteBKG(int W, int H);
	virtual ~CLiteBKG();

private:
	void OnClose();
	virtual void InvalidateCtrl2(RECT& rc);

public:
	void OnPaint();
	void OnMouseMove(const POINT& pt);
	void OnLBtnDown(const POINT& pt);
	void OnLBtnUp();
	void OnMouseWheel(int zDelta);
	void OnRBtnDown(const POINT& pt);
};

