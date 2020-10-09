#include "fmt.h"
#include "env.h"
#include "ui/LiteBKG.h"
#include "ui/zoom/IZoom.h"
#include "ui/zoom/impl/StdMapper.h"
#include "ui/zoom/impl/ZoomFigure.h"
#include "ui/zoom/impl/ZoomKLine.h"


class CMainWnd : public CLiteBKG, public IZoomEventHandler
{
public:
	CMainWnd(int x, int y, int W, int H);
	~CMainWnd();

	void OnClick();
	void OnSelected(bool bSel);
	void OnRadio(int n);
	void OnRadio2(int n);

	void NotifyEvent(int nMsgID, const RECT& rc);
};

CMainWnd::CMainWnd(int x, int y, int W, int H)
	: CLiteBKG(W, H)
{
	int nTop = 10;
	ADD_TXT(10, nTop, 100, 25, "Test Button:", STD_TXT_FMT, RGBH(FFFFFF));
	ADD_BTN(120, nTop, 50, 25, "ok", StdBtn, OnClick);
	nTop += 30;
	ADD_TXT(10, nTop, 100, 25, "Test CheckBox:", STD_TXT_FMT, RGBH(FFFFFF));
	ADD_CHECK_BOX(120, nTop, 50, 25, "check", StdBtn, OnSelected);
	nTop += 30;
	ADD_TXT(10, nTop, 100, 25, "Test RadioBox:", STD_TXT_FMT, RGBH(FFFFFF));
	ADD_RADIO_BOX(120, nTop, 100, 25, "radio.0", StdBtn, OnRadio);
	ADD_RADIO_BOX(220, nTop, 100, 25, "radio.1", StdBtn, OnRadio);
	ADD_RADIO_BOX(320, nTop, 100, 25, "radio.2", StdBtn, OnRadio);
	nTop += 30;
	ADD_TXT(10, nTop, 100, 25, "Test RadioBox 2:", STD_TXT_FMT, RGBH(FFFFFF));
	ADD_RADIO_BOX(120, nTop, 100, 25, "radio2.0", StdBtn, OnRadio2);
	ADD_RADIO_BOX(220, nTop, 100, 25, "radio2.1", StdBtn, OnRadio2);
	ADD_RADIO_BOX(320, nTop, 100, 25, "radio2.2", StdBtn, OnRadio2);

	nTop += 30;
	ADD_TXT(10, nTop, 100, 25, "Test Scroll:", STD_TXT_FMT, RGBH(FFFFFF));
	ADD_SCROLL_HORIZON(120, nTop, 300, 25, 50, StdBtn);

	nTop += 30;
	auto pZoom = ADD_ZOOM(10, nTop, 640, 360);
	pZoom->SetMapper(new CStdMapper);
	auto pFigure = new CZoomFigure;
	pFigure->AddPoint(3, 4, CLR_R, "xxx");
	pFigure->AddPoint(3, 4, CLR_G, "+");
	pFigure->AddPoint(30, 100, CLR_G, "xyz");
	pZoom->AddZoomImpl(pFigure);
	auto pKLine = new CZoomKLine;
	pKLine->Add(1.1, 1.2, 1.0, 1.2);
	pKLine->Add(1.22, 1.25, 1.18, 1.27);
	pKLine->Add(1.26, 1.21, 1.18, 1.27);
	pZoom->AddZoomImpl(pKLine);
	pZoom->SetScroll(pKLine->m_posRT, POINT(630, 10));
}

CMainWnd::~CMainWnd()
{

}

void CMainWnd::OnClick()
{
	cout << "OnClick" << endl;
}

void CMainWnd::OnSelected(bool bSel)
{
	cout << "OnSelected: " << bSel << endl;
}

void CMainWnd::OnRadio(int n)
{
	cout << "OnRadio: " << n << endl;
}

void CMainWnd::OnRadio2(int n)
{
	cout << "OnRadio2: " << n << endl;
}

void CMainWnd::NotifyEvent(int nMsgID, const RECT &rc)
{
	Println("NotifyEvent:", nMsgID, rc);
}

int main()
{
	cout << "Hello World!" << endl;
	new CMainWnd(0, 0, 800, 600);
	gui->Start();
	return 0;
}
