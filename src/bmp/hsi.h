#pragma once
#include "env.h"
#include "pix.h"
#include "geometry/func.h"

struct SHueSI
{
	CDirection<float> h;
	float s, i;
};

class CRgbHsi
{
public:
	PIX32 m_H[256];
	map<int, int> m_mapH;

	CRgbHsi();
	static PIX_HSI RGB2HSI(PIX32 pix);
	static SHueSI RGB2HueSI(int r, int g, int b);
	static SHueSI RGB2HueSI(PIX32 pix);
	static unsigned char RGB2H(PIX32 pix);
};
extern CRgbHsi g_hsi;
