#include "wnd.h"
#include <iostream>
#include <map>
using namespace std;

Display* g_dsp = nullptr;
map<Window, CWnd*> g_mapWnd;
struct InitDisplay
{
	InitDisplay()
	{
		XInitThreads();
		g_dsp = XOpenDisplay(nullptr);
	}
};
InitDisplay init_disp;

bool GetMessage(XEvent& evt)
{
	XEvent tmp;
//	unsigned evt_mask;
	switch (evt.type)
	{
	case Expose:
		while (XCheckWindowEvent(g_dsp, evt.xany.window, ExposureMask, &tmp));
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
	bool b = XNextEvent(g_dsp, &evt) == 0;// calls XFlush()
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

void DispatchMessage(const XEvent& evt)
{
	static int count = 0;
	Window w = evt.xany.window;
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
			g_mapWnd[w]->OnLBtnDown(POINT(evt.xbutton.x, evt.xbutton.y));
			break;
		case 2:
			//g_mapWnd[w]->OnLBtnDown(CPoint(evt.xbutton.x, evt.xbutton.y));
			break;
		case 3:
			g_mapWnd[w]->OnRBtnDown(POINT(evt.xbutton.x, evt.xbutton.y));
			break;
		case 4:
			g_mapWnd[w]->OnMouseWhell(120);
			break;
		case 5:
			g_mapWnd[w]->OnMouseWhell(-120);
			break;
		}
		break;
	case ButtonRelease:
		//cout << "ButtonRelease: " << evt.xbutton.button << endl;
		switch (evt.xbutton.button)
		{
		case 1:
			g_mapWnd[w]->OnLBtnUp(/*CPoint(evt.xbutton.x, evt.xbutton.y)*/);
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
		g_mapWnd[w]->OnMouseMove(POINT(evt.xmotion.x, evt.xmotion.y));
		if (XPending(g_dsp) > 3)
			return;
		break;
	default:
		cout << evt.type << endl;
		return;
		//break;
	}
	g_mapWnd[w]->OnPaint();
	g_mapWnd[w]->Flush();
}

void MessageLoop()
{
	XEvent evt;
	evt.type = -1;
	while (GetMessage(evt))
		DispatchMessage(evt);
}

CWnd::CWnd(int x, int y, int W, int H)
	: CLiteBKG(*this, m_dc, W, H)
	, m_rcInvalidate(0, 0, W, H)
{
//	m_nWidth = W;
//	m_nHeight = H;
//	unsigned long white = WhitePixel(g_dsp, screenNumber);
//	unsigned long black = BlackPixel(g_dsp, screenNumber);
	m_hWnd = XCreateSimpleWindow(g_dsp, DefaultRootWindow(g_dsp), x, y, unsigned(W), unsigned(H), 0, 0x000000, 0xFFFFFF);
	XMapWindow(g_dsp, m_hWnd);
	long eventMask = ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
	XSelectInput(g_dsp, m_hWnd, eventMask);
	g_mapWnd[m_hWnd] = this;

	m_dcWnd.Init(m_hWnd);
	m_dc.CreateCompatible(m_dcWnd);
}

void CWnd::CenterWindow()
{
	int scrNum = DefaultScreen(g_dsp);
	int scrWidth = XDisplayWidth(g_dsp, scrNum);
	int scrHeight = XDisplayHeight(g_dsp, scrNum);
	XMoveWindow(g_dsp, m_hWnd, (scrWidth - m_nWidth) / 2, (scrHeight - m_nHeight) / 2);
}

XEvent event;
void CWnd::SendRefreshEvent()
{
	event.type = Expose;
	event.xany.window = m_hWnd;
	cout << XSendEvent(g_dsp, m_hWnd, 1, ExposureMask, &event) << endl;
//	cout << XPending(g_dsp) << endl;
	cout << XSync(g_dsp, false) << endl;
}

CDC& CWnd::GetDC()
{
	return m_dc;
}

void CWnd::Flush()
{
	//m_dc.Flush(m_rcInvalidate);
	m_dcWnd.BitBlt(m_dc, m_rcInvalidate.left, m_rcInvalidate.top, unsigned(m_rcInvalidate.Width()), unsigned(m_rcInvalidate.Height()), m_rcInvalidate.left, m_rcInvalidate.top);
	m_rcInvalidate.SetRectEmpty();
}

void CWnd::Refresh(RECT &rc)
{
	m_rcInvalidate.UnionRect(m_rcInvalidate, rc);
	//cout << "Refresh" << endl;
}

void CWnd::OnClose()
{
	cout << "OnClose" << endl;
	XDestroyWindow(g_dsp, m_hWnd);
	XCloseDisplay(g_dsp);
}
