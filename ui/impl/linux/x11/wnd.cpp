#include "wnd.h"
#include <iostream>
#include <map>
using namespace std;


CX11Global::CX11Global()
{
	XInitThreads();
	m_dsp = XOpenDisplay(nullptr);
	auto screen = DefaultScreen(m_dsp);
	m_nScreenWidth = DisplayWidth(m_dsp, screen);
	m_nScreenHeight = DisplayHeight(m_dsp, screen);
	printf("ScreenCount: %d, DefaultScreen: %d[%d * %d]\n", ScreenCount(m_dsp), screen, m_nScreenWidth, m_nScreenHeight);
	m_fontDefault = XLoadQueryFont(m_dsp, "-misc-fixed-medium-r-semicondensed--0-0-75-75-c-0-iso8859-9");
}

void CX11Global::Start()
{
	// MessageLoop:
	XEvent evt;
	evt.type = -1;
	while (GetMessage(evt))
		DispatchMessage(evt);
}

ILiteWnd* CX11Global::CreateWindow()
{
	return new CX11Wnd;
}

int CX11Global::ScreenWidth()
{
	return m_nScreenWidth;
}

int CX11Global::ScreenHeight()
{
	return m_nScreenHeight;
}

bool CX11Global::GetMessage(XEvent& evt)
{
	XEvent tmp;
//	unsigned evt_mask;
	switch (evt.type)
	{
	case Expose:
		while (XCheckWindowEvent(m_dsp, evt.xany.window, ExposureMask, &tmp));
		break;
	}
//	while (1)
//	{
//		if (XNextEvent(g_dsp, &evt) != 0)// calls XFlush()
//			return false;
//		switch (evt.type)
//		{
//		case MotionNotify:
//			if (XPending(g_dsp) > 3)
//				continue;
//			break;
//		default:
//			return true;
//		}
//	}
	bool b = XNextEvent(m_dsp, &evt) == 0;// calls XFlush()
//	switch (evt.type)
//	{
//	case MotionNotify:
//		for (int i = 0; i < 10; ++i)
//		{
//			if (XCheckWindowEvent(g_dsp, evt.xany.window, PointerMotionMask, &tmp))
//				evt = tmp;
//			else
//				break;
//		}
//		break;
//	}

	return b;
}

void CX11Global::DispatchMessage(const XEvent& evt)
{
	static int count = 0;
	Window w = evt.xany.window;
	auto pX11Wnd = m_mapWnd[w];
	switch (evt.type)
	{
	case ClientMessage:
		cout << "ClientMessage: " << count++ << endl;
		break;
	case MapNotify:
		break;
	case Expose:
		cout << "Expose: " << count++ << endl;
		break;
	case GraphicsExpose:
		cout << "GraphicsExpose: " << count++ << endl;
		break;
	case NoExpose:
		//cout << "NoExpose" << count++ << endl;
		return;
	case ButtonPress:
		//cout << "ButtonPress: " << evt.xbutton.button << endl;
		switch (evt.xbutton.button)
		{
		case 1:
			pX11Wnd->OnLBtnDown(POINT(evt.xbutton.x, evt.xbutton.y));
			break;
		case 2:
			break;
		case 3:
			pX11Wnd->OnRBtnDown(POINT(evt.xbutton.x, evt.xbutton.y));
			break;
		case 4:
			pX11Wnd->OnMouseWheel(120);
			break;
		case 5:
			pX11Wnd->OnMouseWheel(-120);
			break;
		}
		return;
	case ButtonRelease:
		//cout << "ButtonRelease: " << evt.xbutton.button << endl;
		switch (evt.xbutton.button)
		{
		case 1:
			pX11Wnd->OnLBtnUp(/*CPoint(evt.xbutton.x, evt.xbutton.y)*/);
			break;
		case 2:
			break;
		case 3:
			break;
		}
		return;
	case MotionNotify:
		pX11Wnd->OnMouseMove(POINT(evt.xmotion.x, evt.xmotion.y));
		//if (XPending(m_dsp) > 3)
		//	return;
		return;
	case ReparentNotify:
		// @beginning
		return;
	case ConfigureNotify:
		// drag move window
		return;
	default:
		cout << "evt.type: " << evt.type << endl;
		return;
	}
	pX11Wnd->OnPaint();
	pX11Wnd->Flush();
}

void CX11Wnd::CenterWindow()
{
	XMoveWindow(X11_DSP, m_hWnd,
				(X11_GUI(m_nScreenWidth) - m_rcInvalidate.Width()) / 2,
				(X11_GUI(m_nScreenHeight) - m_rcInvalidate.Height()) / 2);
}


void CX11Wnd::SendRefreshEvent()
{
	static XEvent event;
	event.type = Expose;
	event.xany.window = m_hWnd;
	cout << "XSendEvent(X11_DSP, m_hWnd, 1, ExposureMask, &event): " << XSendEvent(X11_DSP, m_hWnd, 1, ExposureMask, &event) << endl;
//	cout << XPending(g_dsp) << endl;
	cout << "XSync(X11_DSP, false): " << XSync(X11_DSP, false) << endl;
}

void CX11Wnd::Flush()
{
	m_dcWnd.BitBlt(m_dcMem, m_rcInvalidate.left, m_rcInvalidate.top, unsigned(m_rcInvalidate.Width()), unsigned(m_rcInvalidate.Height()), m_rcInvalidate.left, m_rcInvalidate.top);
	m_rcInvalidate.SetRectEmpty();
}

void CX11Wnd::Init()
{
	m_rcInvalidate = RECT(0, 0, m_pBKG->Width(), m_pBKG->Height());
	//	unsigned long white = WhitePixel(g_dsp, screenNumber);
	//	unsigned long black = BlackPixel(g_dsp, screenNumber);
	m_hWnd = XCreateSimpleWindow(X11_DSP, DefaultRootWindow(X11_DSP),
								 0, 0, unsigned(m_pBKG->Width()), unsigned(m_pBKG->Height()),
								 0, 0x000000, 0xFFFFFF);
	XMapWindow(X11_DSP, m_hWnd);
	long eventMask = ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
	XSelectInput(X11_DSP, m_hWnd, eventMask);

	m_dcWnd.Init(m_hWnd);
	m_dcMem.CreateCompatible(m_dcWnd);
	X11_GUI(m_mapWnd)[m_hWnd] = this;
}

ILiteDC* CX11Wnd::GetDC()
{
	return &m_dcMem;
}

void CX11Wnd::Refresh(RECT &rc)
{
	m_rcInvalidate.UnionRect(m_rcInvalidate, rc);
	SendRefreshEvent();
}

void CX11Wnd::OnClose()
{
	cout << "OnClose" << endl;
	XDestroyWindow(X11_DSP, m_hWnd);
	XCloseDisplay(X11_DSP);
}

