#include <iostream>
#include "wnd.h"
#include "zoom/IZoom.h"
#include "zoom/impl/ZoomFigure.h"

using namespace std;

class CLocalImgWnd : public CLiteBKG
{
public:
	CLocalImgWnd(int x, int y, int W, int H);
	~CLocalImgWnd();

	void OnClick();
	void OnSelected(bool bSel);
	void OnRadio(int n);
	void OnRadio2(int n);
};

CLocalImgWnd::CLocalImgWnd(int x, int y, int W, int H)
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
	auto pZoom = ADD_ZOOM(10, nTop, 640, 360, OnClick);
	auto pFigure = new CZoomFigure;
	pFigure->Add(3, 4, CLR_R, "xxx");
	pFigure->Add(3, 4, CLR_G, "+");
	pFigure->Add(30, 40, CLR_G, "xyz");
	pZoom->SetZoomImpl(pFigure);
}

CLocalImgWnd::~CLocalImgWnd()
{

}

void CLocalImgWnd::OnClick()
{
	cout << "OnClick" << endl;
}

void CLocalImgWnd::OnSelected(bool bSel)
{
	cout << "OnSelected: " << bSel << endl;
}

void CLocalImgWnd::OnRadio(int n)
{
	cout << "OnRadio: " << n << endl;
}

void CLocalImgWnd::OnRadio2(int n)
{
	cout << "OnRadio2: " << n << endl;
}

int main()
{
	cout << "Hello World!" << endl;
	gui = new CX11Global;
	new CLocalImgWnd(0, 0, 800, 600);
	gui->Start();
	return 0;
}
