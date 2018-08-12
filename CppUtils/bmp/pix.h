#pragma once

struct PIX32
{
	unsigned char b, g, r, alpha;
	//PIX32() { b = 0; g = 0; r = 0; alpha = 0; }
	//PIX32(unsigned char _r, unsigned char _g, unsigned char _b): b(_b), g(_g), r(_r) {}
	unsigned operator - (PIX32 pix)
	{
		return abs(b - pix.b) + abs(g - pix.g) + abs(r - pix.r);
	}
	operator unsigned long ()
	{
		return *(unsigned long*)this;
	}
};
const PIX32 PIX32_ZERO = { 0 };
const PIX32 PIX32_FFFF = { 255, 255, 255, 255 };
const PIX32 PIX32_R = { 0, 0, 255 };
const PIX32 PIX32_G = { 0, 255, 0 };
const PIX32 PIX32_B = { 255, 0, 0 };
const PIX32 PIX32_C = { 255, 255, 0 };
const PIX32 PIX32_M = { 255, 0, 255 };
const PIX32 PIX32_Y = { 0, 255, 255 };
const PIX32 PIX32_K = { 0, 0, 0 };
const PIX32 PIX32_WHITE = { 255, 255, 255 };
const PIX32 PIX32_GRAY = { 127, 127, 127 };

struct PIX_HSI
{
	unsigned char i, s, h, alpha;
};
