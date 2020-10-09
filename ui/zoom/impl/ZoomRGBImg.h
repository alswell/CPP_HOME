#pragma once
#include "../IZoom.h"
#include "bmp/RGBImg.h"

class CZoomRGBImg : public IZoom
{
	CRGBImg* m_pImg;
public:
	CZoomRGBImg(int nCtrlID, CRGBImg* pImg);
	virtual void Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn);
	virtual RECT GetRect();
	virtual void GetPixInfo(char *buff, int r, int c);

	virtual CMouseCapturer* WantCapture();
	virtual void RBtnDown(POINT pt);
};
