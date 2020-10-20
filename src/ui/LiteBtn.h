#pragma once
#include "LiteCtrlStatic.h"
#include "MouseCapturer.h"


struct BtnStyle
{
	COLORREF clrBkgnd[4];
	COLORREF clrBorder[4];
	COLORREF clrText[4];
	UINT nFormat[4];
};
const BtnStyle StdBtn = {
	{ RGBH(fabf14), RGBH(9ebc2b), RGBH(dae122), RGBH(999999) },
	{ RGBH(dae122), RGBH(dae122), RGBH(9ebc2b), RGBH(333333) },
	{ RGBH(ffffff), RGBH(dae122), RGBH(9ebc2b), RGBH(ffffff) },
	{ TXT_FMT_CENTER | TXT_FMT_VCENTER | TXT_FMT_SINGLELINE | TXT_FMT_END_ELLIPSIS }
};
const BtnStyle TipBtn = {
	{ RGBH(009999), RGBH(ccff99), RGBH(6699cc), RGBH(999999) },
	{ RGBH(00FF00), RGBH(66cc99), RGBH(336699), RGBH(333333) },
	{ RGBH(FFFFFF), RGBH(333333), RGBH(333333), RGBH(ffffff) },
	{ TXT_FMT_CENTER | TXT_FMT_VCENTER | TXT_FMT_END_ELLIPSIS }
};

//enum EnBtnType
//{
//	BTN_TYPE_NORMAL = 0,
//	BTN_SYPE_TAG,
//	BTN_SYPE_RADIO,
//	BTN_SYPE_CHECKBOX,
//};

#define SetNormal() Show(0)
#define SetHover() Show(1)
#define SetDownOrSelect() Show(2)
#define SetDisable() Show(3)
#define ADD_MAGIC_BOX(l, t, W, H, str, style) dynamic_cast<CLiteMagicBox*>(AddCtrl(new CLiteMagicBox(RECT(l, t, l+W, t+H), str, btnStyle)))
#define ADD_TIP(l, t, W, H) (dynamic_cast<CLiteMagicBox*>(AddCtrl(new CLiteMagicBox(RECT(l, t, l+W, t+H), "", TipBtn), 10)))
class CLiteMagicBox : public CMouseCapturer
{
protected:
	const BtnStyle& m_stBtnStyle;
	CColorBlock* m_pBkg;
	CTextBlock* m_pTxt;
	//CTextBlock* m_pIconTxt;
	long m_nCurIndex;
public:
	CLiteMagicBox(const RECT& rcRelLoc, const char* str, const BtnStyle& btnStyle);
	void SetText(char* str);
	void Show(int nIndex, bool refresh = true);
};


#define ADD_BTN(l, t, W, H, str, style, cb) (dynamic_cast<CLiteBtn*>(AddCtrl(new CLiteBtn(RECT(l, t, l+W, t+H), str, style))))->RegHandler(&THIS_CLS::cb)
class CLiteBtn : public CLiteMagicBox
{
protected:
	bool m_bIsDisable;
public:
	typedef void(*NOTIFY)(void* self);
	NOTIFY m_cbNotify;
	CLiteBtn(RECT rcRelLoc, const char* str, const BtnStyle& btnStyle);
	template<class T>
	CLiteBtn* RegHandler(void(T::*cb)()) { m_cbNotify = reinterpret_cast<NOTIFY>(cb); return this; }
	virtual CMouseCapturer* WantCapture();
	virtual void CaptureMouse(bool bDown);
	virtual void ReleaseMouse(bool bDown);
	virtual void Activate(POINT);
	virtual void Inactivate(bool bCapture);
};


#define ADD_CHECK_BOX(l, t, W, H, str, style, cb) (dynamic_cast<CLiteCheckBox*>(AddCtrl(new CLiteCheckBox(RECT(l, t, l+W, t+H), str, style))))->RegHandler(&THIS_CLS::cb)
class CLiteCheckBox : public CLiteMagicBox
{
	bool m_bSelected;
	bool m_bIsDisable;
public:
	typedef void(*NOTIFY)(void* self, bool bSelected);
	NOTIFY m_cbNotify;
	CLiteCheckBox(RECT rcRelLoc, const char* str, const BtnStyle& btnStyle);
	template<class T>
	CLiteCheckBox* RegHandler(void(T::*cb)(bool)) { m_cbNotify = reinterpret_cast<NOTIFY>(cb); return this; }
	virtual CMouseCapturer* WantCapture();
	virtual void CaptureMouse(bool bDown);
	virtual void ReleaseMouse(bool bDown);
	virtual void Activate(POINT);
	virtual void Inactivate(bool bCapture);
};


#define ADD_RADIO_BOX(l, t, W, H, str, style, cb) (dynamic_cast<CLiteRadioBox*>(AddCtrl(new CLiteRadioBox(RECT(l, t, l+W, t+H), str, style))))->RegHandler(&THIS_CLS::cb)
class CLiteRadioBox : public CLiteMagicBox
{
protected:
	bool m_bSelected;
	bool m_bIsDisable;
public:
	typedef void(*NOTIFY)(void* self, int nSelected);
	NOTIFY m_cbNotify;
	CLiteRadioBox(RECT rcRelLoc, const char* str, const BtnStyle& btnStyle);
	template<class T>
	CLiteRadioBox* RegHandler(void(T::*cb)(int)) { m_cbNotify = reinterpret_cast<NOTIFY>(cb); return this; }
	virtual CMouseCapturer* WantCapture();
	virtual void CaptureMouse(bool bDown);
	virtual void ReleaseMouse(bool bDown);
	virtual void Activate(POINT);
	virtual void Inactivate(bool bCapture);
	void Notify(bool call);
	void Select();
};


class CLiteScrollBar : public CLiteBtn
{
	POINT m_ptDown;
public:
	CLiteScrollBar(RECT rcRelLoc, const char* str, const BtnStyle& btnStyle);

	virtual void Activate(POINT ptWnd);
	virtual void ActivateMove(POINT ptWnd);
};
enum EScrollType
{
	SCROLL_NORMAL = 0,
	SCROLL_HORIZON,
	SCROLL_VERTICAL,
};
#define ADD_SCROLL_HORIZON(l, t, W, H, len, style) (dynamic_cast<CLiteScroll*>(AddCtrl(new CLiteScroll(RECT(l, t, l+W, t+H), SCROLL_HORIZON, len, style))))
class CLiteScroll : public CLiteCtrlBase
{
protected:
	bool m_bIsDisable;
	CLiteScrollBar* m_pBtn;
	EScrollType m_typeScroll;

public:
	typedef void(*NOTIFY)(void* self, int l, int t, unsigned w, unsigned h, unsigned W, unsigned H);
	NOTIFY m_cbNotify;
	CLiteScroll(RECT rcRelLoc, EScrollType type, unsigned len, const BtnStyle& btnStyle);
	template<class T>
	CLiteScroll* RegHandler(void(T::*cb)(int, int, unsigned, unsigned, unsigned, unsigned)) { m_cbNotify = reinterpret_cast<NOTIFY>(cb); return this; }
	void SetBtnLen(unsigned len);
};
