#include "LiteBtn.h"


static map<CLiteRadioBox::NOTIFY, vector<void*>> m_mapRadio;

CLiteMagicBox::CLiteMagicBox(CLiteCtrlBase* pParentCtrl, const RECT& rcRelLoc, const char* str, const BtnStyle& btnStyle)
	: CMouseCapturer(pParentCtrl, rcRelLoc)
	, m_stBtnStyle(btnStyle)
	, m_nCurIndex(0)
{
	m_pBkg = ADD_BLOCK(0, 0, rcRelLoc.Width(), rcRelLoc.Height(), btnStyle.clrBkgnd[0], btnStyle.clrBorder[0]);
	int nLeft = 0;
	//	nLeft = 20;
	//	m_pIconTxt = new CTextBlock(RectW(0, (rcRelLoc.Height() - SYMBOL_HEIGHT) / 2 - 2, nLeft, SYMBOL_HEIGHT), RADIO_UNCHECK, btnStyle.clrBorder[0], TXT_FMT_CENTER | TXT_FMT_VCENTER | TXT_FMT_SINGLELINE);
	//	m_pIconTxt->SetFont(FONT_SYMBOL);
	//	AddCtrl(m_pIconTxt);
	m_pTxt = ADD_TXT(nLeft, 0, rcRelLoc.Width(), rcRelLoc.Height(), str, btnStyle.nFormat[0], btnStyle.clrText[0]);
}

void CLiteMagicBox::SetText(char* str)
{
	m_pTxt->SetText(str);
}

void CLiteMagicBox::Show(int nIndex, bool refresh)
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
	if (refresh)
		InvalidateCtrl();
}


CLiteBtn::CLiteBtn(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc, const char* str, const BtnStyle& btnStyle, NOTIFY cb)
	: CLiteMagicBox(pParentCtrl, rcRelLoc, str, btnStyle)
	, m_bIsDisable(false)
	, m_cbNotify(cb)
{

}

CMouseCapturer* CLiteBtn::WantCapture(POINT ptParent)
{
	if (m_bIsDisable)
		return nullptr;
	return CMouseCapturer::WantCapture(ptParent);
}

void CLiteBtn::CaptureMouse(bool bDown)
{
	if (bDown)
		SetDownOrSelect();
	else
		SetHover();
}

void CLiteBtn::ReleaseMouse(bool bDown)
{
	if (bDown)
		SetHover();
	else
		SetNormal();
}

void CLiteBtn::Activate(POINT)
{
	SetDownOrSelect();
}

void CLiteBtn::Inactivate(bool bCapture)
{
	if (bCapture)
	{
		SetHover();
		if (m_cbNotify)
			reinterpret_cast<NOTIFY>(m_cbNotify)(m_pParentCtrl);
	}
	else
	{
		SetNormal();
	}
}


CLiteCheckBox::CLiteCheckBox(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc, const char* str, const BtnStyle& btnStyle, NOTIFY cb)
	: CLiteMagicBox(pParentCtrl, rcRelLoc, str, btnStyle)
	, m_bSelected(false)
	, m_bIsDisable(false)
	, m_cbNotify(cb)
{
}

CMouseCapturer* CLiteCheckBox::WantCapture(POINT ptParent)
{
	if (m_bIsDisable)
		return nullptr;
//	if (m_bIsSelected)
//		return NULL;
	return CMouseCapturer::WantCapture(ptParent);
}

void CLiteCheckBox::CaptureMouse(bool bDown)
{
	if (!m_bSelected)
	{
		if (bDown)
			SetDownOrSelect();
		else
			SetHover();
	}
}

void CLiteCheckBox::ReleaseMouse(bool bDown)
{
	if (!m_bSelected)
	{
		if (bDown)
			SetHover();
		else
			SetNormal();
	}
}

void CLiteCheckBox::Activate(POINT)
{
	SetDownOrSelect();
}

void CLiteCheckBox::Inactivate(bool bCapture)
{
	if (bCapture)
	{
		m_bSelected = !m_bSelected;
		if (m_cbNotify)
			reinterpret_cast<NOTIFY>(m_cbNotify)(m_pParentCtrl, m_bSelected);
		if (m_bSelected)
			SetDownOrSelect();
		else
			SetHover();
	}
	else
	{
		if (!m_bSelected)
			SetNormal();
	}
}

CLiteRadioBox::CLiteRadioBox(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc, const char* str, const BtnStyle& btnStyle, NOTIFY cb)
	: CLiteMagicBox(pParentCtrl, rcRelLoc, str, btnStyle)
	, m_bSelected(false)
	, m_bIsDisable(false)
	, m_cbNotify(cb)
{
	m_mapRadio[m_cbNotify].push_back(this);
}

CMouseCapturer* CLiteRadioBox::WantCapture(POINT ptParent)
{
	if (m_bIsDisable)
		return nullptr;
	//		if (m_bIsSelected)
	//			return NULL;
	return CMouseCapturer::WantCapture(ptParent);
}

void CLiteRadioBox::CaptureMouse(bool bDown)
{
	if (!m_bSelected)
	{
		if (bDown)
			SetDownOrSelect();
		else
			SetHover();
	}
}

void CLiteRadioBox::ReleaseMouse(bool bDown)
{
	if (!m_bSelected)
	{
		if (bDown)
			SetHover();
		else
			SetNormal();
	}
}

void CLiteRadioBox::Activate(POINT)
{
	SetDownOrSelect();
}

void CLiteRadioBox::Inactivate(bool bCapture)
{
	if (bCapture)
	{
		if (m_bSelected)
			return;
		m_bSelected = true;
		Notify(true);
		if (m_bSelected)
			SetDownOrSelect();
		else
			SetHover();
	}
	else
	{
		if (!m_bSelected)
			SetNormal();
	}
}

void CLiteRadioBox::Notify(bool call)
{
	unsigned n = 0;
	auto& v = m_mapRadio[m_cbNotify];
	for (unsigned i = 0; i < v.size(); ++i) {
		if (v[i] == this)
			n = i;
		else {
			auto p = reinterpret_cast<CLiteRadioBox*>(v[i]);
			p->m_bSelected = false;
			p->SetNormal();
		}
	}
	if (call)
		reinterpret_cast<NOTIFY>(m_cbNotify)(m_pParentCtrl, int(n));
}

void CLiteRadioBox::Select()
{
	m_bSelected = true;
	Show(2, false);
	Notify(false);
}

CLiteScrollBar::CLiteScrollBar(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc, const char* str, const BtnStyle& btnStyle)
	: CLiteBtn(pParentCtrl, rcRelLoc, str, btnStyle, nullptr)
{

}

void CLiteScrollBar::Activate(POINT ptWnd)
{
	CLiteBtn::Activate(ptWnd);
	m_ptDown = ParentToChild(ptWnd); // WindowToChild
}

void CLiteScrollBar::ActivateMove(POINT ptWnd)
{
	auto pt = ParentToChild(ptWnd); // WindowToChild
	int x = pt.x - m_ptDown.x;
	int y = pt.y - m_ptDown.y;
	RECT rcTmp = m_rcRelLoc;

	rcTmp.OffsetRect(x, y);
	if (rcTmp.left < 0)
		x = 0;
	else if (rcTmp.right > m_pParentCtrl->Width())
		x = m_pParentCtrl->Width() - Width();
	else
		x = rcTmp.left;
	if (rcTmp.top < 0)
		y = 0;
	else if (rcTmp.bottom > m_pParentCtrl->Height())
		y = m_pParentCtrl->Height() - Height();
	else
		y = rcTmp.top;
	MoveTo(x, y);
}

CLiteScroll::CLiteScroll(CLiteCtrlBase* pParentCtrl, RECT rcRelLoc, EScrollType type, unsigned len, const BtnStyle& btnStyle, CLiteScroll::NOTIFY cb)
	: CLiteCtrlBase(rcRelLoc, pParentCtrl)
	, m_typeScroll(type)
	, m_cbNotify(cb)
{
	ADD_BLOCK2(GetDrawRect(), RGBH(009900), RGBH(990000));
	if (m_typeScroll == SCROLL_HORIZON)
		m_pBtn = new CLiteScrollBar(this, RECT(0, 0, len, rcRelLoc.Height()), nullptr, btnStyle);
	else
		m_pBtn = new CLiteScrollBar(this, RECT(0, 0, rcRelLoc.Width(), len), nullptr, btnStyle);
	AddCtrl(m_pBtn);
}

void CLiteScroll::SetBtnLen(unsigned len)
{
}
