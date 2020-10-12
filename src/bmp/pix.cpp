#include "pix.h"

double Distance(PIX32 pix1, PIX32 pix2)
{
	double delta_r = pix1.r - pix2.r;
	double delta_g = pix1.g - pix2.g;
	double delta_b = pix1.b - pix2.b;
	return sqrt(SQUARE(delta_r) + SQUARE(delta_g) + SQUARE(delta_b));
}

const char *ToStr(PIX32 pix)
{
	static char buff[64];
	sprintf(buff, "[%3d, %3d, %3d]", pix.r, pix.g, pix.b);
	return buff;
}
