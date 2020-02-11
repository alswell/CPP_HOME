#pragma once
#include "LiteCtrlStatic.h"
#include "LiteBtn.h"

enum KEYBOARD
{
	KEY_UNKNOWN = 0,
	KEY_CTRL_L,
	KEY_CTRL_R,
	KEY_SHIFT_L,
	KEY_SHIFT_R,
	KEY_A,
};
extern KEYBOARD g_nKeyMap[256];

class CLiteBKG;
class ILiteWnd
{
protected:
	CLiteBKG* m_pBKG;
public:
	virtual ~ILiteWnd();
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
	void OnKeyDown(KEYBOARD key);
	void OnKeyUp(KEYBOARD key);
};
class ILiteGlobal
{
	friend class CLiteBKG;
	ILiteWnd* NewWnd(CLiteBKG* pBKG);
public:
	virtual ~ILiteGlobal();
	virtual void Start() = 0;
	virtual ILiteWnd* CreateWindow() = 0;
	virtual int ScreenWidth() = 0;
	virtual int ScreenHeight() = 0;
};
extern ILiteGlobal* gui;

class CLiteBKG : public CLiteCtrlBase
{
protected:
	bool m_bKeyDown[256];
	CColorBlock* m_pBKG;
	CLiteMagicBox* m_pTip;

	CMouseCapturer* m_pHoverCtrl;
	CMouseCapturer* m_pDownCtrl;
	//static CBJICtrlBase* m_pDownActiveCtrl;
	//static map<CBJICtrlBase*, BOOL> m_pActiveMenu;

	ILiteWnd* m_implContext;
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
	void OnKeyDown(KEYBOARD key);
	void OnKeyUp(KEYBOARD key);

	bool KeyDown(KEYBOARD key);
};
#define ROOT_CTRL(p) reinterpret_cast<CLiteBKG*>(p)
