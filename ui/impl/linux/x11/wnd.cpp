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
	auto pCtxt = m_mapContext[w];
	auto pBKG = pCtxt->m_pBKG;
	switch (evt.type)
	{
	case ClientMessage:
		cout << "ClientMessage" << count++ << endl;
		break;
	case MapNotify:
		//g_mapWnd[w]->InvalidateCtrl();
		break;
	case Expose:
		cout << "Expose" << count++ << endl;
		break;
	case GraphicsExpose:
		cout << "GraphicsExpose" << count++ << endl;
		break;
	case NoExpose:
		//cout << "NoExpose" << count++ << endl;
		return;
		//break;
	case ButtonPress:
		//cout << "ButtonPress: " << evt.xbutton.button << endl;
		switch (evt.xbutton.button)
		{
		case 1:
			pBKG->OnLBtnDown(POINT(evt.xbutton.x, evt.xbutton.y));
			break;
		case 2:
			//g_mapWnd[w]->OnLBtnDown(CPoint(evt.xbutton.x, evt.xbutton.y));
			break;
		case 3:
			pBKG->OnRBtnDown(POINT(evt.xbutton.x, evt.xbutton.y));
			break;
		case 4:
			pBKG->OnMouseWhell(120);
			break;
		case 5:
			pBKG->OnMouseWhell(-120);
			break;
		}
		break;
	case ButtonRelease:
		//cout << "ButtonRelease: " << evt.xbutton.button << endl;
		switch (evt.xbutton.button)
		{
		case 1:
			pBKG->OnLBtnUp(/*CPoint(evt.xbutton.x, evt.xbutton.y)*/);
			break;
		case 2:
			//g_mapWnd[w]->OnLBtnDown(CPoint(evt.xbutton.x, evt.xbutton.y));
			break;
		case 3:
			//g_mapWnd[w]->OnLBtnDown(CPoint(evt.xbutton.x, evt.xbutton.y));
			break;
		}
		break;
	case MotionNotify:
		pBKG->OnMouseMove(POINT(evt.xmotion.x, evt.xmotion.y));
		if (XPending(m_dsp) > 3)
			return;
		break;
	case ConfigureNotify:
		// drag move window
		return;
	default:
		cout << evt.type << endl;
		return;
		//break;
	}
	pBKG->OnPaint();
	pCtxt->Flush();
}

void CX11Global::MessageLoop()
{
	XEvent evt;
	evt.type = -1;
	while (GetMessage(evt))
		DispatchMessage(evt);
}

void CX11Global::Start()
{
	MessageLoop();
}

ILiteContext* CX11Global::GetContext(CLiteBKG* pBKG)
{
	auto p = new CX11Context(pBKG);
	return p;
}

int CX11Global::ScreenWidth()
{
	return m_nScreenWidth;
}

int CX11Global::ScreenHeight()
{
	return m_nScreenHeight;
}

CX11Context::CX11Context(CLiteBKG* pBKG)
	: m_pBKG(pBKG)
	, m_rcInvalidate(0, 0, pBKG->m_nWidth, pBKG->m_nHeight)
{
//	m_nWidth = W;
//	m_nHeight = H;
//	unsigned long white = WhitePixel(g_dsp, screenNumber);
//	unsigned long black = BlackPixel(g_dsp, screenNumber);
	m_hWnd = XCreateSimpleWindow(X11_DSP, DefaultRootWindow(X11_DSP),
								 0, 0, unsigned(pBKG->m_nWidth), unsigned(pBKG->m_nHeight),
								 0, 0x000000, 0xFFFFFF);
	XMapWindow(X11_DSP, m_hWnd);
	long eventMask = ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
	XSelectInput(X11_DSP, m_hWnd, eventMask);

	m_dcWnd.Init(m_hWnd);
	m_dc.CreateCompatible(m_dcWnd);
	X11_GUI(m_mapContext)[m_hWnd] = this;
}

void CX11Context::CenterWindow()
{
	XMoveWindow(X11_DSP, m_hWnd,
				(X11_GUI(m_nScreenWidth) - m_rcInvalidate.Width()) / 2,
				(X11_GUI(m_nScreenHeight) - m_rcInvalidate.Height()) / 2);
}


void CX11Context::SendRefreshEvent()
{
	static XEvent event;
	event.type = Expose;
	event.xany.window = m_hWnd;
	cout << XSendEvent(X11_DSP, m_hWnd, 1, ExposureMask, &event) << endl;
//	cout << XPending(g_dsp) << endl;
	cout << XSync(X11_DSP, false) << endl;
}

void CX11Context::Flush()
{
	//m_dc.Flush(m_rcInvalidate);
	m_dcWnd.BitBlt(m_dc, m_rcInvalidate.left, m_rcInvalidate.top, unsigned(m_rcInvalidate.Width()), unsigned(m_rcInvalidate.Height()), m_rcInvalidate.left, m_rcInvalidate.top);
	m_rcInvalidate.SetRectEmpty();
}

ILiteDC* CX11Context::GetDC()
{
	return &m_dc;
}

void CX11Context::Refresh(RECT &rc)
{
	m_rcInvalidate.UnionRect(m_rcInvalidate, rc);
	//cout << "Refresh" << endl;
	SendRefreshEvent();
}

void CX11Context::OnClose()
{
	cout << "OnClose" << endl;
	XDestroyWindow(X11_DSP, m_hWnd);
	XCloseDisplay(X11_DSP);
}

