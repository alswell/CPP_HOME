#pragma once
#include "LiteBKG.h"

#define ZOOM_MULTI(x, m) (m > 0 ? x * m : x / -m)
#define ZOOM_DIVID(x, m) (m > 0 ? x / m : x * -m)
class IBmpMapper
{
public:
	int m_nMulti;
	IBmpMapper(int nMulti = 1);
	virtual ~IBmpMapper();
	void Zoom(int iDelta);

	virtual float GetZoom();
	virtual void Map(float& x, float& y) = 0;
	virtual void Revert(float& x, float& y) = 0;
};

class IZoom : public CMouseCapturer
{
	friend class CZoomView;
protected:
	int m_nCtrlID;
	IBmpMapper* m_implBmpMapper;
public:
	IZoom(int nCtrlID = 0);
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
	IZoom* m_vZooms[2];
	IZoom* m_implZoom;
	IZoom* m_implEdit;
	POINT m_ptDown;
	POINT m_ptCoordinate;
	char m_strCoordinate[128];
public:
	CZoomView();

	virtual CMouseCapturer* WantCapture(POINT ptParent);
	virtual void Activate(POINT ptWnd);
	virtual void ActivateMove(POINT ptWnd);
	virtual void MouseWheel(int zDelta);
	virtual char* GetTipString();
	virtual void RBtnDown(POINT pt);

	void SetZoomImpl(IZoom* implZoom);
	POINT PixCoordinate(const POINT& pt);
	void SetCoordinate();
	POINT GetCoordinate();
	char* StrCoordinate();
private:
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

#define ADD_ZOOM(l, t, W, H) ((CZoom*)AddCtrl(new CZoom(RECT(l, t, l+W, t+H))))
class IZoomEventHandler
{
public:
	virtual void NotifyEvent(int nMsgID, const Point<int>& pt);
	virtual void NotifyEvent(int nMsgID, const RECT& rc);
};
class CZoom : public CLiteCtrlBase
{
	friend class CMultiView;
protected:
	CCoordinateH* m_pCoordinateH;
	CCoordinateV* m_pCoordinateV;
	CZoomView* m_pZoomView;
public:
	CZoom(RECT rcRelLoc);

	void SetZoomImpl(IZoom* implZoom);
	void NotifyOffset();
	POINT GetCoordinate();

	void NotifyEvent(int nMsgID, const Point<int>& pt);
	void NotifyEvent(int nMsgID, const RECT& rc);
};
