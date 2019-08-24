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

#define ADD_BTN(l, t, W, H, str, style, cb) (dynamic_cast<CLiteBtn<THIS_CLS>*>(AddCtrl(new CLiteBtn<THIS_CLS>(this, RECT(l, t, l+W, t+H), str, style, &THIS_CLS::cb))))
#define ADD_TIP(l, t, W, H) (dynamic_cast<CLiteBtn<CLiteCtrlBase>*>(AddCtrl(new CLiteBtn<CLiteCtrlBase>(this, RECT(l, t, l+W, t+H), "", TipBtn, NULL), 10)))
template <class T_HANDLER>
class CLiteBtn : public CMouseCapturer
{
protected:
	bool m_bIsDisable;
	const BtnStyle& m_stBtnStyle;
	CColorBlock* m_pBkg;
	CTextBlock* m_pTxt;
	int m_nCurIndex;

	typedef void(T_HANDLER::*HANDLE_CLICK)();
	HANDLE_CLICK m_funClick;
public:
	CLiteBtn(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc, const char* str, const BtnStyle& btnStyle, HANDLE_CLICK cb_click)
		: CMouseCapturer(pParentCtrl, rcRelLoc)
		, m_stBtnStyle(btnStyle)
		, m_bIsDisable(false)
		, m_nCurIndex(0)
		, m_funClick(cb_click)
	{
		m_pBkg = ADD_BLOCK2(GetDrawRect(), btnStyle.clrBkgnd[0], btnStyle.clrBorder[0]);
		int nLeft = 0;
		m_pTxt = ADD_TXT(nLeft, 0, rcRelLoc.Width(), rcRelLoc.Height(), str, btnStyle.nFormat[0], btnStyle.clrText[0]);
	}
	void SetText(char* str) { m_pTxt->SetText(str); }
	void Show(int nIndex)
	{
		if (m_nCurIndex == nIndex)
			return;

		m_pBkg->SetColor(m_stBtnStyle.clrBkgnd[nIndex], m_stBtnStyle.clrBorder[nIndex]);
		m_pTxt->SetColor(m_stBtnStyle.clrText[nIndex]);
		m_nCurIndex = nIndex;
		InvalidateCtrl();
	}

	virtual CMouseCapturer* WantCapture(POINT pt)
	{
		if (m_bIsDisable)
			return nullptr;
		return CMouseCapturer::WantCapture(pt);
	}
	virtual void CaptureMouse()
	{
		SetHover();
	}
	virtual void ReleaseMouse(bool bDown)
	{
		if (bDown)
			SetHover();
		else
			SetNormal();
	}
	virtual void Activate(POINT pt)
	{
		SetDownOrSelect();
	}
	virtual void Inactivate(bool bCapture)
	{
		if (bCapture)
		{
			SetHover();
			(dynamic_cast<T_HANDLER*>(m_pParentCtrl)->*m_funClick)();
		}
		else
		{
			SetNormal();
		}
	}
};

#define ADD_CHECK_BOX(l, t, W, H, str, style, cb) (dynamic_cast<CLiteCheckBox<THIS_CLS>*>(AddCtrl(new CLiteCheckBox<THIS_CLS>(this, RECT(l, t, l+W, t+H), str, style, &THIS_CLS::cb))))
template <class T_HANDLER>
class CLiteCheckBox : public CMouseCapturer
{
protected:
	bool m_bSelected;
	bool m_bIsDisable;
	const BtnStyle& m_stBtnStyle;
	CColorBlock* m_pBkg;
	CTextBlock* m_pTxt;
	int m_nCurIndex;

	typedef void(T_HANDLER::*HANDLE_CLICK)(bool bSelected);
	HANDLE_CLICK m_funClick;
public:
	CLiteCheckBox(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc, const char* str, const BtnStyle& btnStyle, HANDLE_CLICK cb_click)
		: CMouseCapturer(pParentCtrl, rcRelLoc)
		, m_stBtnStyle(btnStyle)
		, m_bSelected(false)
		, m_bIsDisable(false)
		, m_nCurIndex(0)
		, m_funClick(cb_click)
	{
		m_pBkg = ADD_BLOCK2(GetDrawRect(), btnStyle.clrBkgnd[0], btnStyle.clrBorder[0]);
		int nLeft = 0;
		m_pTxt = ADD_TXT(nLeft, 0, rcRelLoc.Width(), rcRelLoc.Height(), str, btnStyle.nFormat[0], btnStyle.clrText[0]);
	}
	void SetText(char* str) { m_pTxt->SetText(str); }
	void Show(int nIndex)
	{
		if (m_nCurIndex == nIndex)
			return;

		m_pBkg->SetColor(m_stBtnStyle.clrBkgnd[nIndex], m_stBtnStyle.clrBorder[nIndex]);
		m_pTxt->SetColor(m_stBtnStyle.clrText[nIndex]);
		m_nCurIndex = nIndex;
		InvalidateCtrl();
	}

	virtual CMouseCapturer* WantCapture(POINT pt)
	{
		if (m_bIsDisable)
			return nullptr;
//		if (m_bIsSelected)
//			return NULL;
		return CMouseCapturer::WantCapture(pt);
	}
	virtual void CaptureMouse()
	{
		if (!m_bSelected)
			SetHover();
	}
	virtual void ReleaseMouse(bool bDown)
	{
		if (!m_bSelected)
		{
			if (bDown)
				SetHover();
			else
				SetNormal();
		}
	}
	virtual void Activate(POINT pt)
	{
		SetDownOrSelect();
	}
	virtual void Inactivate(bool bCapture)
	{
		if (bCapture)
		{
			m_bSelected = !m_bSelected;
			(dynamic_cast<T_HANDLER*>(m_pParentCtrl)->*m_funClick)(m_bSelected);
			if (!m_bSelected)
				SetHover();
		}
		else
		{
			if (!m_bSelected)
				SetNormal();
		}
	}
};

extern map<void*, vector<void*>> m_mapRadio;
#define ADD_RADIO_BOX(l, t, W, H, str, style, cb) (dynamic_cast<CLiteRadioBox<THIS_CLS>*>(AddCtrl(new CLiteRadioBox<THIS_CLS>(this, RECT(l, t, l+W, t+H), str, style, &THIS_CLS::cb))))
template <class T_HANDLER>
class CLiteRadioBox : public CMouseCapturer
{
protected:
	bool m_bSelected;
	bool m_bIsDisable;
	const BtnStyle& m_stBtnStyle;
	CColorBlock* m_pBkg;
	CTextBlock* m_pTxt;
	//CTextBlock* m_pIconTxt;
	int m_nCurIndex;

	typedef void(T_HANDLER::*HANDLE_CLICK)(int nSelected);
	HANDLE_CLICK m_funClick;
public:
	CLiteRadioBox(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc, const char* str, const BtnStyle& btnStyle, HANDLE_CLICK cb_click)
		: CMouseCapturer(pParentCtrl, rcRelLoc)
		, m_stBtnStyle(btnStyle)
		, m_bSelected(false)
		, m_bIsDisable(false)
		, m_nCurIndex(0)
		, m_funClick(cb_click)
	{
		m_mapRadio[reinterpret_cast<void*>(m_funClick)].push_back(this);
		m_pBkg = ADD_BLOCK2(GetDrawRect(), btnStyle.clrBkgnd[0], btnStyle.clrBorder[0]);
		int nLeft = 0;
		//	nLeft = 20;
		//	m_pIconTxt = new CTextBlock(RectW(0, (rcRelLoc.Height() - SYMBOL_HEIGHT) / 2 - 2, nLeft, SYMBOL_HEIGHT), RADIO_UNCHECK, btnStyle.clrBorder[0], TXT_FMT_CENTER | TXT_FMT_VCENTER | TXT_FMT_SINGLELINE);
		//	m_pIconTxt->SetFont(FONT_SYMBOL);
		//	AddCtrl(m_pIconTxt);
		m_pTxt = ADD_TXT(nLeft, 0, rcRelLoc.Width(), rcRelLoc.Height(), str, btnStyle.nFormat[0], btnStyle.clrText[0]);
	}
	void SetText(char* str) { m_pTxt->SetText(str); }
	void Show(int nIndex)
	{
		if (m_nCurIndex == nIndex)
			return;

		m_pBkg->SetColor(m_stBtnStyle.clrBkgnd[nIndex], m_stBtnStyle.clrBorder[nIndex]);
		m_pTxt->SetColor(m_stBtnStyle.clrText[nIndex]);
		//	m_pBkg->SetColor(m_stBtnStyle.clrBkgnd[nIndex == 3 ? 3 : 0], m_stBtnStyle.clrBorder[nIndex]);
		//	m_pIconTxt->SetColor(m_stBtnStyle.clrBorder[nIndex]);
		//	if (nIndex == 2)
		//		m_pIconTxt->SetText(RADIO_CHECKED);
		//	else if (nIndex != 3)
		//		m_pIconTxt->SetText(RADIO_UNCHECK);
		m_nCurIndex = nIndex;
		InvalidateCtrl();
	}

	virtual CMouseCapturer* WantCapture(POINT pt)
	{
		if (m_bIsDisable)
			return nullptr;
//		if (m_bIsSelected)
//			return NULL;
		return CMouseCapturer::WantCapture(pt);
	}
	virtual void CaptureMouse()
	{
		if (!m_bSelected)
			SetHover();
	}
	virtual void ReleaseMouse(bool bDown)
	{
		if (!m_bSelected)
		{
			if (bDown)
				SetHover();
			else
				SetNormal();
		}
	}
	virtual void Activate(POINT pt)
	{
		SetDownOrSelect();
	}
	virtual void Inactivate(bool bCapture)
	{
		if (bCapture)
		{
			m_bSelected = !m_bSelected;
			int n = 0;
			auto& v = m_mapRadio[reinterpret_cast<void*>(m_funClick)];
			for (int i = 0; i < v.size(); ++i) {
				if (v[i] == this)
					n = i;
				else {
					auto p = reinterpret_cast<CLiteRadioBox*>(v[i]);
					p->m_bSelected = false;
					p->SetNormal();
				}
			}
			(dynamic_cast<T_HANDLER*>(m_pParentCtrl)->*m_funClick)(n);
			if (!m_bSelected)
				SetHover();
		}
		else
		{
			if (!m_bSelected)
				SetNormal();
		}
	}
};
