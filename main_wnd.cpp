#include <iostream>
#include "wnd.h"

using namespace std;

class CLocalImgWnd : public CLiteBKG
{
	CONF_THIS_CLS(CLocalImgWnd);
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
