#pragma once
#include <X11/Xlib.h>
#include "dc.h"
#include "LiteBKG.h"

#define X11_GUI(x) dynamic_cast<CX11Global*>(gui)->x
#define X11_DSP X11_GUI(m_dsp)

class CX11Global : public ILiteGlobal
{
	friend class CX11Context;
	friend class CX11DC;
	Display* m_dsp;
	map<Window, CX11Context*> m_mapContext;
	int m_nScreenWidth;
	int m_nScreenHeight;
	XFontStruct* m_fontDefault;
public:
	CX11Global();
	bool GetMessage(XEvent& evt);
	void DispatchMessage(const XEvent& evt);
	void MessageLoop();
	virtual void Start();
	virtual ILiteContext* GetContext(CLiteBKG* pBKG);
	virtual int ScreenWidth();
	virtual int ScreenHeight();
};

class CX11Context : public ILiteContext
{
public:
	Window m_hWnd;
	CLiteBKG* m_pBKG;
//	unsigned m_nWidth;
//	unsigned m_nHeight;
	CX11DC m_dcWnd;
	CX11DC m_dc;
	RECT m_rcInvalidate;
public:
	CX11Context(CLiteBKG* pBKG);
	void CenterWindow();
//	void SendRefreshEvent();
	void Flush();

	virtual ILiteDC* GetDC();
	virtual void Refresh(RECT& rc);
	virtual void OnClose();
};
