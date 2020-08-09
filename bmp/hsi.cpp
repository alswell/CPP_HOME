#include "hsi.h"

CRgbHsi g_hsi;
CRgbHsi::CRgbHsi()
{
	int temp_h[256];
	memset(m_H, 0, sizeof(m_H));
	memset(temp_h, 0, sizeof(temp_h));
	PIX_HSI hsi;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			PIX32 pixes[6] = {
				{ static_cast<unsigned char>(i), static_cast<unsigned char>(j), 0, 0 },
				{ static_cast<unsigned char>(i), static_cast<unsigned char>(j), 255, 0 },
				{ static_cast<unsigned char>(i), 0, static_cast<unsigned char>(j), 0 },
				{ static_cast<unsigned char>(i), 255, static_cast<unsigned char>(j), 0 },
				{ 0, static_cast<unsigned char>(i), static_cast<unsigned char>(j), 0 },
				{ 255, static_cast<unsigned char>(i), static_cast<unsigned char>(j), 0 }
			};
			for (int k = 0; k < 6; k++)
			{
				hsi = RGB2HSI(pixes[k]);
				if (temp_h[hsi.h] < hsi.s)
				{
					temp_h[hsi.h] = hsi.s;
					m_H[hsi.h] = pixes[k];
				}
			}
			//hsi = RGB2HSI(PIX32(i, j, 0));
			//hsi = RGB2HSI(PIX32(i, j, 255));
			//hsi = RGB2HSI(PIX32(i, 0, j));
			//hsi = RGB2HSI(PIX32(i, 255, j));
			//hsi = RGB2HSI(PIX32(0, i, j));
			//hsi = RGB2HSI(PIX32(255, i, j));
		}
	}
	for (int i = 0; i < 256; i++)
	{
		m_mapH.insert(make_pair(*reinterpret_cast<int*>(m_H + i), i));
	}
}

PIX_HSI CRgbHsi::RGB2HSI(PIX32 pix)
{
	PIX_HSI hsi;
	CVector3<float> pt(pix.r, pix.g, pix.b);
	pt.Rotation3DZSelf(float(PI_4));
	pt.Rotation3DXSelf(float(0.955316663));
	float dir = CDirection<float>(CVector2<float>(pt.x, pt.y));
	hsi.h = static_cast<unsigned char>((double(dir) + PI) * 255 / PI_D);
	hsi.s = static_cast<unsigned char>(CG::Distance(CVector2<float>(0, 0), CVector2<float>(pt.x, pt.y)) * sqrt(3.f) / sqrt(2.f));
	hsi.i = static_cast<unsigned char>(pt.z / sqrt(3.f));
	return hsi;
}

SHueSI CRgbHsi::RGB2HueSI(int r, int g, int b)
{
	CVector3<float> pt(r, g, b);
	pt.Rotation3DZSelf(float(PI_4));
	pt.Rotation3DXSelf(float(0.955316663));
	return {
		CDirection<float>(CVector2<float>(pt.x, pt.y)),
		CG::Distance(CVector2<float>(0, 0), CVector2<float>(pt.x, pt.y)) * sqrt(3.f) / sqrt(2.f),
		pt.z / sqrt(3.f),
	};
}

SHueSI CRgbHsi::RGB2HueSI(PIX32 pix)
{
	return RGB2HueSI(pix.r, pix.g, pix.b);
}

unsigned char CRgbHsi::RGB2H(PIX32 pix)
{
	CVector3<float> pt(pix.r, pix.g, pix.b);
	pt.Rotation3DZSelf(float(PI_4));
	pt.Rotation3DXSelf(float(0.955316663));
	float dir = CDirection<float>(CVector2<float>(pt.x, pt.y));
	return static_cast<unsigned char>(((double(dir) + PI) * 255 / PI_D));
}
