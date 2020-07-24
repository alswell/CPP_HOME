#pragma once
#include "LiteBKG.h"

class IBmpMapper
{
public:
	int m_nMulti;
	IBmpMapper(int nMulti = 1);
	virtual ~IBmpMapper();
	void Zoom(int iDelta);

	virtual void Map(float& x, float& y) = 0;
	virtual void Revert(float& x, float& y) = 0;
};

class IZoom : public CLiteCtrlBase
{
	friend class CZoomView;
protected:
	IBmpMapper* m_implBmpMapper;
public:
	virtual ~IZoom();
	virtual void Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn) = 0;
	virtual RECT GetRect() = 0;
	virtual void GetPixInfo(char *buff, int x, int y) = 0;

	void Zoom(int iDelta);
	void Map(float& x, float& y);
	void Revert(float& x, float& y);
	int GetMulti();
	void ResetRect();

	void NotifyEvent(int nMsgID, const Point<int>& pt);
	void NotifyEvent(int nMsgID, const RECT& rc);
};


class CZoomView : public CMouseCapturer
{
	friend class CZoom;
	friend class CMultiView;
	IZoom* m_implZoom;
	bool m_bDown;
	CColorBlock* m_pRedRect;
	RECT m_rcRealRedRect;
	POINT m_ptDown;
	POINT m_ptCoordinate;
	char m_strCoordinate[128];
public:
	CZoomView();

	virtual CMouseCapturer* WantCapture(POINT ptParent);
	virtual void Activate(POINT ptWnd);
	virtual void ActivateMove(POINT ptWnd);
	virtual void Inactivate(bool bCapture);
	virtual void MouseWheel(int zDelta);
	virtual char* GetTipString();
	virtual void RBtnDown(POINT pt);

	void SetZoomImpl(IZoom* implZoom);
	POINT PixCoordinate(const POINT& pt);
	void SetCoordinate();
	POINT GetCoordinate();
	char* StrCoordinate();
private:
	void SetRedRect();
	void ResetScroll();
};

class CCoordinate : public CLiteCtrlBase
{
protected:
	char m_strCoordinate[8];
	int m_nBegin;
	int m_nMulti;
public:
	CCoordinate();

	void Update(int nScroll, int nMulti);
};

class CCoordinateH : public CCoordinate
{
public:
	virtual void Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn);
};

class CCoordinateV : public CCoordinate
{
public:
	virtual void Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn);
};

#define ADD_ZOOM(l, t, W, H, cb) ((CZoom*)AddCtrl(new CZoom(RECT(l, t, l+W, t+H), reinterpret_cast<CZoom::NOTIFY_RBTN_DOWN>(&this->cb))))
#define REG_RED_RECT_CB(zoom, cb) zoom->RegRedRectCB(reinterpret_cast<CZoom::NOTIFY_RBTN_DOWN>(&this->cb))
class CZoom : public CLiteCtrlBase
{
	friend class CMultiView;
protected:
	CCoordinateH* m_pCoordinateH;
	CCoordinateV* m_pCoordinateV;
	CZoomView* m_pZoomView;
public:
	typedef void(*NOTIFY_RBTN_DOWN)(void* self, Point<int> pt);
	NOTIFY_RBTN_DOWN m_cbNotifyRBtnDown;
	typedef void(*NOTIFY_RED_RECT)(void* self, const RECT& rc);
	NOTIFY_RED_RECT m_cbNotifyRedRect;

	CZoom(RECT rcRelLoc, NOTIFY_RBTN_DOWN cb = nullptr);
	void RegRedRectCB(NOTIFY_RED_RECT cb);

	void SetZoomImpl(IZoom* implZoom);
	void NotifyOffset();
	void NotifyRedRect(const RECT& rc);
	void NotifyRBtnDown();
	POINT GetCoordinate();

	typedef void(*EVENT_PT)(void* self, int nMsgID, const Point<int>& pt);
	EVENT_PT m_cbEventPt;
	void NotifyEvent(int nMsgID, const Point<int>& pt);
	typedef void(*EVENT_RECT)(void* self, int nMsgID, const RECT& rc);
	EVENT_RECT m_cbEventRect;
	void NotifyEvent(int nMsgID, const RECT& rc);
};
