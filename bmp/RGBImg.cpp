#include "RGBImg.h"
#include "pix.h"

CRGBImg::CRGBImg()
{
}

CRGBImg::CRGBImg(unsigned uRow, unsigned uColumn, PIX32 * pT)
	: CMatrix<PIX32>(uRow, uColumn, pT)
{
}

CRGBImg::CRGBImg(unsigned uRow, unsigned uColumn, PIX32 t)
	: CMatrix<PIX32>(uRow, uColumn, t) 
{
}

void CRGBImg::Expand(unsigned expand)
{
	expand += expand;
	m_uRow += expand;
	m_uColumn += expand;
	if (m_pT)
		delete[] m_pT;
	m_pT = new PIX32[m_uRow * m_uColumn];
}

void CRGBImg::ToR()
{
	for (unsigned i = 0; i < m_uRow * m_uColumn; i++)
	{
		m_pT[i].g = m_pT[i].r;
		m_pT[i].b = m_pT[i].r;
	}
}

void CRGBImg::ToG()
{
	for (unsigned i = 0; i < m_uRow * m_uColumn; i++)
	{
		m_pT[i].r = m_pT[i].g;
		m_pT[i].b = m_pT[i].g;
	}
}

void CRGBImg::ToB()
{
	for (unsigned i = 0; i < m_uRow * m_uColumn; i++)
	{
		m_pT[i].g = m_pT[i].b;
		m_pT[i].r = m_pT[i].b;
	}
}

void CRGBImg::DrawCross(int r, int c)
{
	for (int i = r - 5; i <= r + 5; ++i)
		(*this)[i][c] = PIX32_K;
	for (int i = c - 5; i <= c + 5; ++i)
		(*this)[r][i] = PIX32_K;
}

void CRGBImg::DrawOutline(IIterator<PointF*>& iter)
{
	ITER(p, iter)
		(*this)[int(p->x)][int(p->y)] = PIX32_B;
}


//CGradient CRGBImg::Gradient(int r, int c, int channel)
//{
//	int center = RGB_CH(r, c, channel);
//	int delta00 = RGB_CH(r - 1, c - 1, channel);// - RGB_CH(r, c, channel);
//	int delta11 = RGB_CH(r + 1, c + 1, channel);// - RGB_CH(r, c, channel);
//	int delta01 = RGB_CH(r - 1, c + 1, channel);// - RGB_CH(r, c, channel);
//	int delta10 = RGB_CH(r + 1, c - 1, channel);// - RGB_CH(r, c, channel);
//
//	float x = 0;
//	x += RGB_CH(r - 1, c, channel);
//	x -= RGB_CH(r + 1, c, channel);
//
//	x += delta00 * sin(PI_4);
//	x += delta01 * sin(PI_4);
//	x -= delta10 * sin(PI_4);
//	x -= delta11 * sin(PI_4);
//
//	float y = 0;
//	y += RGB_CH(r, c - 1, channel);
//	y -= RGB_CH(r, c + 1, channel);
//
//	y += delta00 * sin(PI_4);
//	y += delta10 * sin(PI_4);
//	y -= delta01 * sin(PI_4);
//	y -= delta11 * sin(PI_4);
//
//	return CGradient(x, y);
//}
//
//void CRGBImg::Gradients(CMatrix<CGradient>& _out_grad, int channel)
//{
//	_out_grad.Init(m_uRow, m_uColumn);
//	for (int r = 1; r < m_uRow - 1; ++r)
//	{
//		for (int c = 1; c < m_uColumn; ++c)
//		{
//			_out_grad[r][c] = Gradient(r, c);
//		}
//	}
//}
//
//
//CRgbHsi g_rgbhsi;
//
//void CRGB32Img::ToH()
//{
//	for (int i = 0; i < m_uRow * m_uColumn; i++)
//	{
//		PIX_HSI hsi = CRgbHsi::RGB2HSI(m_pT[i]);
//		m_pT[i].r = hsi.h;
//		m_pT[i].g = m_pT[i].r;
//		m_pT[i].b = m_pT[i].r;
//		m_pT[i].alpha = hsi.s;
//	}
//}
//
//void CRGB32Img::ToI()
//{
//	for (int i = 0; i < m_uRow * m_uColumn; i++)
//	{
//		PIX_HSI hsi = CRgbHsi::RGB2HSI(m_pT[i]);
//		m_pT[i].r = hsi.i /*/ SCALE_I * SCALE_I*/;
//		m_pT[i].g = m_pT[i].r;
//		m_pT[i].b = m_pT[i].r;
//		m_pT[i].alpha = hsi.h;
//	}
//}
//
//void CRGB32Img::Scale(CRGB32Img& _out, int low, int range)
//{
//	//low %= range;
//	_out = *this;
//	for (int i = 0; i < m_uRow * m_uColumn; i++)
//	{
//		int value = m_pT[i].r;
//		if (value < low || value >(low + range))
//			value = 255;
//		else
//			value = 0;
//		//_out.m_pT[i].r = value > low ? (value - low) / range * range + low : 0;
//		_out.m_pT[i].r = value;
//		_out.m_pT[i].g = _out.m_pT[i].r;
//		_out.m_pT[i].b = _out.m_pT[i].r;
//	}
//}
//
//void CRGB32Img::Devide(CRGB32Img& _out, int low)
//{
//	_out = *this;
//	for (int i = 0; i < m_uRow * m_uColumn; i++)
//	{
//		int value = m_pT[i].r;
//		//_out_pix[i].r = (value + low) / SCALE_I * SCALE_I;
//		//_out_pix[i].g = _out_pix[i].r;
//		//_out_pix[i].b = _out_pix[i].r;
//
//		if (value < low)
//		{
//			_out.m_pT[i].r = 0;
//			_out.m_pT[i].g = 0;
//			_out.m_pT[i].b = 0;
//		}
//		else
//		{
//			_out.m_pT[i].r = 255;
//			_out.m_pT[i].g = 255;
//			_out.m_pT[i].b = 255;
//		}
//		//else if (value > high)
//		//{
//		//	_out_pix[i].r = 255;
//		//	_out_pix[i].g = 255;
//		//	_out_pix[i].b = 255;
//		//}
//		//else
//		//{
//		//	_out_pix[i].r = 0;
//		//	_out_pix[i].g = 127;
//		//	_out_pix[i].b = 0;
//		//}
//	}
//
//}
//
