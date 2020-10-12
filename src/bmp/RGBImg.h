#pragma once
// #include "ImgClass.h"
// #include "Border.h"
#include "pix.h"
#include "matrix.h"
#include "iter.h"

#define CHANNEL_PIX 0
#define CHANNEL_ALPHA 3
#define CHANNEL_R 2
#define CHANNEL_G 1
#define CHANNEL_B 0
#define RGB_CH(r, c, channel) (*((reinterpret_cast<unsigned char*>(m_pT + unsigned(r) * (m_uColumn) + unsigned(c))) + channel))
class CRGBImg : public CMatrix<PIX32>
{
public:
	CRGBImg();
	CRGBImg(unsigned uRow, unsigned uColumn, PIX32* pT = nullptr);
	CRGBImg(unsigned uRow, unsigned uColumn, PIX32 t);

public:
	void Expand(unsigned expand);

	template <class DES_T>
	void ExtractChannel(CMatrix<DES_T>& _out_m, int channel)
	{
		_out_m = CMatrix<DES_T>(m_uRow, m_uColumn);
		for (unsigned r = 0; r < m_uRow; r++)
			for (unsigned c = 0; c < m_uColumn; c++)
				_out_m[r][c] = DES_T(RGB_CH(r, c, channel));
	}

	template <class SRC_T>
	void FillChannel(const CMatrix<SRC_T>& _in_m, int channel)
	{
		Init(_in_m.m_uRow, _in_m.m_uColumn);
		unsigned char temp;
		for (unsigned r = 0; r < m_uRow; r++)
			for (unsigned c = 0; c < m_uColumn; c++)
			{
				temp = _in_m[r][c];
				if (_in_m[r][c] > 255)
					temp = 255;
				else if (_in_m[r][c] < 0)
					temp = 0;
				RGB_CH(r, c, channel) = temp;
			}
	}

	template <class SRC_T>
	void CreateGrey(const CMatrix<SRC_T>& _in_m)
	{
		Init(_in_m.m_uRow, _in_m.m_uColumn);
		unsigned char temp;
		for (unsigned r = 0; r < m_uRow; r++)
			for (unsigned c = 0; c < m_uColumn; c++)
			{
				temp = _in_m[r][c];
				if (_in_m[r][c] > 255)
					temp = 255;
				else if (_in_m[r][c] < 0)
					temp = 0;
				RGB_CH(r, c, 0) = temp;
				RGB_CH(r, c, 1) = temp;
				RGB_CH(r, c, 2) = temp;
			}
	}

//	template <class SRC_T>
//	void CreateBlackWhite(const CMatrix<SRC_T>& _in_m, int threshold = 130)
//	{
//		int nSrcRow = _in_m.m_uRow;
//		int nSrcColumn = _in_m.m_uColumn;
//		SetSize(_in_m.m_uRow, _in_m.m_uColumn);
//		for (int r = 0; r < nSrcRow; r++)
//			for (int c = 0; c < nSrcColumn; c++)
//				memset(CRGBImg_this_pI(r, c, CHANNEL_PIX), _in_m[r][c] < threshold ? -1 : 0, sizeof(PIX32));
//	}

//	template <class SRC_T>
//	void DrawIsoheight(const CMatrix<SRC_T>& _in_m, int expend)
//	{
//		int nSrcRow = _in_m.m_uRow;
//		int nSrcColumn = _in_m.m_uColumn;
//		for (int r = expend, i = 0; i < nSrcRow; r++, i++)
//			for (int c = expend, j = 0; j < nSrcColumn; c++, j++)
//			{
//				int test;
//				if (_in_m[i][j] < 60)
//					test = 0;
//				else if (_in_m[i][j] < 70)
//					RGB_CH(r, c, CHANNEL_R) = -1;
//				else if (_in_m[i][j] < 80)
//					RGB_CH(r, c, CHANNEL_G) = -1;
//				else if (_in_m[i][j] < 90)
//					RGB_CH(r, c, CHANNEL_B) = -1;
//				//else if (_in_m[i][j] < 100)
//				//	RGB_CH(r, c, CHANNEL_R) = -1;
//				//else if (_in_m[i][j] < 120)
//				//	RGB_CH(r, c, CHANNEL_G) = -1;
//				//else if (_in_m[i][j] < 140)
//				//	RGB_CH(r, c, CHANNEL_B) = -1;
//				else
//					memset(CRGBImg_this_pI(r, c, CHANNEL_B), -1, 4);
//				//memcpy(CRGBImg_this_pI(r, c, CHANNEL_B), &test, 4);
//			}
//	}
	void ToR();
	void ToG();
	void ToB();
	void DrawCross(int r, int c);
	void DrawOutline(IIterator<PointF*>& iter);

	//CGradient Gradient(int r, int c, int channel = CHANNEL_B);
	//void Gradients(CMatrix<CGradient>& _out_grad, int channel = CHANNEL_B);
};

//#define SCALE_I 40
//class CRGB32Img : public CRGBImg
//{
//public:
//	CRGB32Img() {}
//	CRGB32Img(unsigned int uRow, unsigned int uColumn, PIX32* pT = 0) : CRGBImg(uRow, uColumn, pT) {}
//	CRGB32Img(unsigned int uRow, unsigned int uColumn, PIX32 t) : CRGBImg(uRow, uColumn, t) {}
//
//	void ToH();
//	void ToI();
//	void Scale(CRGB32Img& _out, int low, int range = SCALE_I);
//	void Devide(CRGB32Img& _out, int low);
//};
