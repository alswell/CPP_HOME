#pragma once
#include "RGBImg.h"

template <class Info>
class CRGBImgEx : public CRGBImg
{
public:
	CMatrix<Info> m_pInfo;
public:
	CRGBImgEx() {}
	~CRGBImgEx() {
	}

	CRGBImgEx(unsigned int uRow, unsigned int uColumn, PIX32* pT = 0)
		: CRGBImg(uRow, uColumn, pT)
		, m_pInfo(uRow, uColumn, Info()) {}

	CRGBImgEx(unsigned int uRow, unsigned int uColumn, PIX32 t)
		: CRGBImg(uRow, uColumn, t)
		, m_pInfo(uRow, uColumn, Info()) {}

	CRGBImgEx(const CRGBImgEx<Info>& img)
		: CRGBImg(img)
		, m_pInfo(img.m_pInfo) {}

	CRGBImgEx(const CRGBImg& img)
		: CRGBImg(img)
		, m_pInfo(m_uRow, m_uColumn, Info()) {}

	CRGBImgEx<Info>& operator = (const CRGBImg& img)
	{
		this->CRGBImg::operator = (img);
		m_pInfo = CMatrix<Info>(m_uRow, m_uColumn, Info());
		return *this;
	}

	CRGBImgEx<Info>& operator = (const CRGBImgEx<Info>& img)
	{
		this->CRGBImg::operator = (img);
		m_pInfo = img.m_pInfo;
		return *this;
	}
//	void SetSize(unsigned int uRow, unsigned int uColumn)
//	{
//		CRGBImg::SetSize(uRow, uColumn);
//		m_pInfo.Init(uRow, uColumn);
//	}
//	void Init(unsigned int uRow, unsigned int uColumn)
//	{
//		CRGBImg::Init(uRow, uColumn);
//		m_pInfo.Init(uRow, uColumn);
//	}
};


//struct StHSIImgInfo
//{
//	int h;
//};
//class CHSIImg : public CRGBImgEx<StHSIImgInfo>
//{
//public:
//	CHSIImg& operator = (const CRGBImg& img)
//	{
//		this->__super::operator = (img);
//		return *this;
//	}
//	void ToHSI()
//	{
//		m_strAlpha = L"S";
//		for (int i = 0; i < m_uRow * m_uColumn; i++)
//		{
//			PIX_HSI hsi = CRgbHsi::RGB2HSIEx(m_pT[i]);
//			if (hsi.s < 5)
//			{
//				m_pT[i].r = hsi.i;
//				m_pT[i].g = m_pT[i].r;
//				m_pT[i].b = m_pT[i].r;
//			}
//			else
//			{
//				m_pT[i] = CRgbHsi::Instance()->m_H[hsi.h];
//			}
//			m_pT[i].alpha = hsi.s;
//			m_pInfo.m_pT[i].h = hsi.h;
//		}
//	}
//	virtual void Devide(CRGBImg& _out_pix, int low)
//	{
//		_out_pix = *this;
//		for (int i = 0; i < m_uRow * m_uColumn; i++)
//		{
//			//PIX32 pix = m_pT[i];
//			//pix.alpha = 0;
//			unsigned char value = m_pInfo.m_pT[i].h;
//			value = m_pT[i].alpha;
//			if (value < low)
//			{
//				_out_pix.m_pT[i].r = m_pT[i].alpha;
//				_out_pix.m_pT[i].g = _out_pix.m_pT[i].r;
//				_out_pix.m_pT[i].b = _out_pix.m_pT[i].r;
//			}
//			else
//			{
//				_out_pix.m_pT[i] = m_pT[i];
//			}
//		}
//	}
//};
