#pragma once
#include <X11/Xlib.h>
#include "dc.h"
#include "../../../LiteBKG.h"

#define X11_GUI(x) dynamic_cast<CX11Global*>(gui)->x
#define X11_DSP X11_GUI(m_dsp)

class CX11Global : public ILiteGlobal
{
	friend class CX11Wnd;
	friend class CX11DC;
	Display* m_dsp;
	map<Window, CX11Wnd*> m_mapWnd;
	int m_nScreenWidth;
	int m_nScreenHeight;
	XFontStruct* m_fontDefault;
public:
	CX11Global();
	void ListFonts();
	void PrintFontPath();
	virtual void Start();
	virtual ILiteWnd* CreateWindow();
	virtual int ScreenWidth();
	virtual int ScreenHeight();
	bool GetMessage(XEvent& evt);
	void DispatchMessage(const XEvent& evt);
};

class CX11Wnd : public ILiteWnd
{
public:
	Window m_hWnd;
public:
	void CenterWindow();

	virtual void Init();
	virtual ILiteDC* GetDC();
	virtual void Refresh();
	virtual void OnClose();
};
