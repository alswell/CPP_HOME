#include "ZoomRGBImg.h"
#include "bmp/hsi.h"


CZoomRGBImg::CZoomRGBImg(int nCtrlID, CRGBImg* pImg)
	: IZoom(nCtrlID)
{
	m_pImg = pImg;
}

void CZoomRGBImg::Draw(ILiteDC* dc, const RECT& rcLoc, const RECT& rcViewRgn)
{
	if (!m_implBmpMapper)
		return;

	PIX32* pOutImg = reinterpret_cast<PIX32*>(dc->BeginData(rcViewRgn.left, rcViewRgn.top, unsigned(rcViewRgn.Width()), unsigned(rcViewRgn.Height())));
	int I = rcViewRgn.top - rcLoc.top;
	int J = rcViewRgn.left - rcLoc.left;

	for (int r = I, R = 0; R < rcViewRgn.Height(); r++, R++, pOutImg += rcViewRgn.Width())
	{
		for (int c = J, C = 0; C < rcViewRgn.Width(); c++, C++)
		{
			float fR = r, fC = c;
			m_implBmpMapper->Map(fR, fC);
			if (m_pImg->IsInside(int(fR), int(fC)))
				pOutImg[C] = (*m_pImg)[int(fR)][int(fC)];
		}
	}
	dc->EndData();
}

RECT CZoomRGBImg::GetRect()
{
	if (m_implBmpMapper->m_nMulti > 0)
		return RECT(0, 0, int(m_pImg->m_uColumn) * m_implBmpMapper->m_nMulti, int(m_pImg->m_uRow) * m_implBmpMapper->m_nMulti);
	return RECT(0, 0, int(m_pImg->m_uColumn) / -m_implBmpMapper->m_nMulti, int(m_pImg->m_uRow) / -m_implBmpMapper->m_nMulti);
}

void CZoomRGBImg::GetPixInfo(char* buff, int r, int c)
{
	PIX32 pix = PIX32_ZERO;
	if (m_pImg->IsInside(r, c))
		pix = (*m_pImg)[r][c];
	SHueSI hsi = CRgbHsi::RGB2HueSI(pix.r, pix.g, pix.b);
	sprintf(buff, "\n(%3d, %3d, %3d)%3d\n(%.2f, %d, %d)", pix.r, pix.g, pix.b, pix.alpha, double(hsi.h), int(hsi.s), int(hsi.i));
}

CMouseCapturer* CZoomRGBImg::WantCapture()
{
	return nullptr;
}

void CZoomRGBImg::RBtnDown(POINT pt)
{
	pt = WindowToChild(pt);
	float x = pt.x, y = pt.y;
	m_implBmpMapper->Map(y, x);
	NotifyEvent(ZOOM_NOTIFY_PT, POINT(int(x), int(y)));
}
