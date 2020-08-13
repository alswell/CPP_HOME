#pragma once
#include "LiteBKG.h"

#define ZOOM_NOTIFY_PT   110
#define ZOOM_NOTIFY_RECT 111

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
	void Map(int& x, int& y);
	void Map(POINT& pt);
	void Map(PointF& pt);
	void Revert(int& x, int& y);
	void Revert(POINT& pt);
	void Revert(PointF& pt);
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

	void SetMapper(IBmpMapper* p);
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
	IBmpMapper* m_implBmpMapper;
	vector<IZoom*> m_vZooms;
	POINT m_ptDown;
	POINT m_ptCoordinate;
	char m_strCoordinate[128];
public:
	CZoomView();

	virtual CMouseCapturer* WantCapture();
	virtual void Activate(POINT ptWnd);
	virtual void ActivateMove(POINT ptWnd);
	virtual void MouseWheel(int zDelta);
	virtual char* GetTipString();
	virtual void RBtnDown(POINT pt);

	void AddZoomImpl(IZoom* implZoom);
	void SetCoordinate();
	void SetScroll(const POINT &ptTarget, const POINT &ptPos);
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

	void SetMapper(IBmpMapper* p);
	void AddZoomImpl(IZoom* implZoom);
	void SetScroll(const POINT &ptTarget, const POINT &ptPos);
	void NotifyOffset();

	void NotifyEvent(int nMsgID, const Point<int>& pt);
	void NotifyEvent(int nMsgID, const RECT& rc);
};
