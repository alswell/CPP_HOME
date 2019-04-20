#pragma once
#include <X11/Xlib.h>
#include "dc.h"
#include "LiteBKG.h"

extern Display* g_dsp;

bool GetMessage(XEvent& evt);
void DispatchMessage(const XEvent& evt);
void MessageLoop();


class CWnd : public CLiteInterface, public CLiteBKG
{
	Window m_hWnd;
//	unsigned m_nWidth;
//	unsigned m_nHeight;
	CDC m_dcWnd;
	CDC m_dc;
	RECT m_rcInvalidate;
public:
	CWnd(int x, int y, int W, int H);
	void CenterWindow();
	void SendRefreshEvent();
	CDC &GetDC();

	void Flush();

	virtual void Refresh(RECT& rc);
	virtual void OnClose();
};
