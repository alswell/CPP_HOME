#pragma once
#include <assert.h>
#include <vector>
#include <list>
#include <map>
using namespace std;
#include "geometry/vector2.h"

#define IS_INSIDE(r, c) (r >= 0 && r < m_uRow && c >= 0 & c < m_uColumn)
#define IS_OUTSIDE(r, c) (r < 0 || r >= m_uRow || c < 0 || c >= m_uColumn)
#define IS_INSIDE_MX(mx, r, c) (r >= 0 && r < (mx).m_uRow && c >= 0 && c < (mx).m_uColumn)
#define IS_OUT_OF_MX(mx, r, c) (r < 0 || unsigned(r) >= (mx).m_uRow || c < 0 || unsigned(c) >= (mx).m_uColumn)
#define IS_OUT_OF_MX_MARGIN(mx, margin, r, c) (r < margin || unsigned(r) >= ((mx).m_uRow - margin) || c < margin || unsigned(c) >= ((mx).m_uColumn - margin))

#define MAT_ITEM(r, c) (*(m_pT + unsigned(r) * (m_uColumn) + unsigned(c)))
#define MATpITEM(r, c) (m_pT + unsigned(r) * (m_uColumn) + unsigned(c))
#define MATmITEM(m, r, c) (*((m).m_pT + unsigned(r) * ((m).m_uColumn) + unsigned(c)))

template <class T>
class SMatrix
{
public:
	T* pT;
	unsigned uRow;
	unsigned uColumn;
};

template <class T>
class CMatrix
{
public:
	T* m_pT;
	unsigned int m_uRow;
	unsigned int m_uColumn;
public:
	T* operator[] (unsigned long u) const
	{
		assert(u < m_uRow);
		return m_pT + u * m_uColumn;
	}
	T* operator[] (unsigned u) const
	{
		assert(u < m_uRow);
		return m_pT + u * m_uColumn;
	}
	T* operator[] (int i) const
	{
		assert(i >= 0 && i < m_uRow);
		return m_pT + i * m_uColumn;
	}
	T* operator[] (CVector2<int> pt) const
	{
		assert(pt.x >= 0 && pt.x < m_uRow);
		return m_pT + pt.x * m_uColumn + pt.y;
	}

public:	// Constructor & Calculations

	CMatrix(void) : m_pT(0), m_uRow(0), m_uColumn(0) {}
	~CMatrix(void) { if (m_pT) delete[] m_pT; }

	CMatrix(const CMatrix<T>& m)
	{
		m_uRow = m.m_uRow;
		m_uColumn = m.m_uColumn;
		m_pT = new T[m_uRow * m_uColumn];
		memcpy(m_pT, m.m_pT, m_uRow * m_uColumn * sizeof(T));
	}

	CMatrix(unsigned int uRow, unsigned int uColumn, T* pT = 0)
	{
		m_uRow = uRow; 
		m_uColumn = uColumn; 
		m_pT = new T[uRow * uColumn];
		if (pT)
			memcpy(m_pT, pT, uRow * uColumn * sizeof(T));
		else
			memset(m_pT, 0, uRow * uColumn * sizeof(T));
	}

	CMatrix(unsigned int uRow, unsigned int uColumn, T t)
	{
		m_uRow = uRow; 
		m_uColumn = uColumn; 
		m_pT = new T[uRow * uColumn];
		for (int r = 0; r < uRow; r++)
			for (int c = 0; c < uColumn; c++)
				*(m_pT + r * m_uColumn + c) = t;
	}

//	void Init(unsigned int uRow, unsigned int uColumn, T* pT)
//	{
//		if (m_pT)
//			delete[] m_pT;
//		m_pT = pT;
//		m_uRow = uRow;
//		m_uColumn = uColumn;
//	}

	SMatrix<T> Attach(unsigned int uRow, unsigned int uColumn, T* pT)
	{
		SMatrix<T> mx = {m_pT, m_uRow, m_uColumn};
		m_pT = pT;
		m_uRow = uRow;
		m_uColumn = uColumn;
		return mx;
	}

	SMatrix<T> Detach()
	{
		SMatrix<T> mx = {m_pT, m_uRow, m_uColumn};
		m_pT = 0;
		m_uRow = 0;
		m_uColumn = 0;
		return mx;
	}

	void Zero()
	{
		memset(m_pT, 0, m_uRow * m_uColumn * sizeof(T));
	}
	void One()
	{
		memset(m_pT, -1, m_uRow * m_uColumn * sizeof(T));
	}

	void SetSize(unsigned int uRow, unsigned int uColumn)
	{
		if (m_uRow != uRow || m_uColumn != uColumn)
		{
			if (Size() != uRow * uColumn)
			{
				if (m_pT)
					delete[] m_pT;
				m_pT = new T[uRow * uColumn];
			}
			m_uRow = uRow;
			m_uColumn = uColumn;
		}
	}
	void Init(unsigned int uRow, unsigned int uColumn)
	{
		SetSize(uRow, uColumn);
		Zero();
	}
	template<class T2>
	void SetSizeAndInit(const CMatrix<T2>& m)
	{
		SetSize(m.m_uRow, m.m_uColumn);
		Zero();
	}

	unsigned Size() const
	{
		return m_uRow * m_uColumn;
	}

	bool IsInside(int r, int c)
	{
		if (r >= 0 && r < m_uRow && c >= 0 && c < m_uColumn)
			return true;
		return false;
	}


	CMatrix<T>& operator= (const CMatrix<T>& m)
	{
		if (m_uRow != m.m_uRow || m_uColumn != m.m_uColumn)
		{
			if (Size() != m.Size())
			{
				if (m_pT)
					delete[] m_pT;
				m_pT = new T[m.m_uRow * m.m_uColumn];
			}
			m_uRow = m.m_uRow;
			m_uColumn = m.m_uColumn;
		}
		memcpy(m_pT, m.m_pT, m_uRow * m_uColumn * sizeof(T));
		return *this;
	}

	CMatrix<T> operator+ (const CMatrix<T>& m) const
	{
		CMatrix<T> _out_m;
		if (m_uRow == m.m_uRow && m_uColumn == m.m_uColumn)
		{
			_out_m = *this;
			for (int r = 0; r < m_uRow; r++)
				for (int c = 0; c < m_uColumn; c++)
					*(_out_m.m_pT + r * m_uColumn + c) += *(m.m_pT + r * m_uColumn + c);
		}
		return _out_m;
	}

	CMatrix<T> operator+ (T t) const
	{
		CMatrix<T> _out_m(*this);
		for (int r = 0; r < m_uRow; r++)
			for (int c = 0; c < m_uColumn; c++)
				*(_out_m.m_pT + r * m_uColumn + c) += t;
		return _out_m;
	}

	CMatrix<T> operator- () const
	{
		CMatrix<T> _out_m(*this);
		for (int r = 0; r < m_uRow; r++)
			for (int c = 0; c < m_uColumn; c++)
				*(_out_m.m_pT + r * m_uColumn + c) = -*(_out_m.m_pT + r * m_uColumn + c);
		return _out_m;
	}

	CMatrix<T> operator- (const CMatrix<T>& m) const
	{
		CMatrix<T> _out_m;
		if (m_uRow == m.m_uRow && m_uColumn == m.m_uColumn)
		{
			_out_m = *this;
			for (int r = 0; r < m_uRow; r++)
				for (int c = 0; c < m_uColumn; c++)
					*(_out_m.m_pT + r * m_uColumn + c) -= *(m.m_pT + r * m_uColumn + c);
		}
		return _out_m;
	}

	CMatrix<T> operator- (T t) const
	{
		T negative_t = -t;
		return *this + negative_t;
	}

	CMatrix<T> operator* (const CMatrix<T>& m) const
	{
		CMatrix<T> _out_m;
		if (m_uColumn == m.m_uRow)
		{
			_out_m = CMatrix<T>(m_uRow, m.m_uColumn);
			for (int r = 0; r < m_uRow; r++)
				for (int c = 0; c < m.m_uColumn; c++)
					for (int k = 0; k < m_uColumn; k++)
						*(_out_m.m_pT + r * m.m_uColumn + c) += *(m_pT + r * m_uColumn + k) * *(m.m_pT + k * m.m_uColumn + c);
		}
		return _out_m;
	}

	CMatrix<T> operator* (T t) const
	{
		CMatrix<T> _out_m(*this);
		for (int r = 0; r < m_uRow; r++)
			for (int c = 0; c < m_uColumn; c++)
				*(_out_m.m_pT + r * m_uColumn + c) *= t;
		return _out_m;
	}

	template <class DES_T>
	operator CMatrix<DES_T> ()
	{
		CMatrix<DES_T> _out_m(m_uRow, m_uColumn);
		for (int r = 0; r < m_uRow; r++)
			for (int c = 0; c < m_uColumn; c++)
				_out_m[r][c] = (DES_T)MAT_ITEM(r, c);
		return _out_m;
	}
public:	// Get Data
	T* operator() (unsigned r1, unsigned r2, unsigned c1, unsigned c2) const
	{
		T* p = new T[(r2 - r1) * (c2 - c1)];
		int i = 0;
		for (unsigned r = r1; r < r2; ++r)
			for (unsigned c = c1; c < c2; ++c)
				p[i++] = MAT_ITEM(r, c);

		return p;
	}
	//T GetElement(unsigned int r, unsigned int c, int iIndexBegin = 0) const { return *(m_pT + (r - iIndexBegin) * m_uColumn + (c - iIndexBegin)); }
	void GetElements(CMatrix<T>& _out_m, const vector<unsigned int>& vRows, const vector<unsigned int>& vColumns) const
	{
		unsigned int nRow = vRows.size();
		if (nRow == 0)
			nRow = m_uRow;
		unsigned int nColumn = vColumns.size();
		if (nColumn == 0)
			nColumn = m_uColumn;
		_out_m = CMatrix<T>(nRow, nColumn);
		unsigned int r;
		unsigned int c;
		for (int i = 0; i < nRow; i++)
		{
			r = vRows.size() == 0 ? i : vRows[i];
			for (int j = 0; j < nColumn; j++)
			{
				c = vColumns.size() == 0 ? j : vColumns[j];
				*(_out_m.m_pT + i * _out_m.m_uColumn + j) = *(m_pT + r * m_uColumn + c); 
			}
		}
	}

	void GetRow(vector<T>& _out_v_r, unsigned int r, int span = 1) const
	{
		_out_v_r.resize((m_uColumn - 1) / span + 1);
		for (int c = 0, i = 0; c < m_uColumn; c += span, i++)
			_out_v_r[i] = MAT_ITEM(r, c);
	}
	void GetRow(list<T>& _out_v_r, unsigned int r, int span = 1) const
	{
		for (int c = 0; c < m_uColumn; c += span)
			_out_v_r.push_back(MAT_ITEM(r, c));
	}

	void GetColumn(T*& _out_v_c, unsigned& n, unsigned int c, int span = 1) const
	{
		n = (m_uRow - 1) / span + 1;
		_out_v_c = new T[n];
		for (int r = 0, i = 0; r < m_uRow; r += span, i++)
			_out_v_c[i] = MAT_ITEM(r, c);
	}
	void GetColumn(vector<T>& _out_v_c, unsigned int c, int span = 1) const
	{
		_out_v_c.resize((m_uRow - 1) / span + 1);
		for (int r = 0, i = 0; r < m_uRow; r += span, i++)
			_out_v_c[i] = MAT_ITEM(r, c);
	}
	void GetColumn(list<T>& _out_v_c, unsigned int c, int span = 1) const
	{
		for (int r = 0; r < m_uRow; r += span)
			_out_v_c.push_back(MAT_ITEM(r, c));
	}

public:
	void SetElement(unsigned int r, unsigned int c, T t, int iIndexBegin = 0) { *(m_pT + (r - iIndexBegin) * m_uColumn + (c - iIndexBegin)) = t; }
	void SetElements(const vector<unsigned int>& vRows, const vector<unsigned int>& vColumns, const CMatrix<T>& m, int iIndexBegin = 0)
	{
		unsigned int nRow = vRows.size();
		if (nRow == 0)
			nRow = m_uRow;
		unsigned int nColumn = vColumns.size();
		if (nColumn == 0)
			nColumn = m_uColumn;
		unsigned int r;
		unsigned int c;
		for (int i = 0; i < nRow; i++)
		{
			r = vRows.size() == 0 ? i : vRows[i] - iIndexBegin;
			for (int j = 0; j < nColumn; j++)
			{
				c = vColumns.size() == 0 ? j : vColumns[j] - iIndexBegin;
				*(m_pT + r * m_uColumn + c) = *(m.m_pT + i * m.m_uColumn + j); 
			}
		}
	}

public:
	void ShiftRowSwap(unsigned int r1, unsigned int r2)
	{
		T temp;
		for (int i = 0; i < m_uColumn; i++)
		{
			temp = MAT_ITEM(r1, i);
			MAT_ITEM(r1, i) = MAT_ITEM(r2, i);
			MAT_ITEM(r2, i) = temp;
		}
	}

	void ShiftRowTimes(unsigned int r, T t)
	{
		for (int i = 0; i < m_uColumn; i++)
			MAT_ITEM(r, i) *= t;
	}

	void ShiftRowOffset(unsigned int _des_r, unsigned int _src_r, T t)
	{
		for (int i = 0; i < m_uColumn; i++)
			MAT_ITEM(_des_r, i) += MAT_ITEM(_src_r, i) * t;
	}

public:
	void Expend(CMatrix<T>& _out_m, int resolution)
	{
		int nSrcRow = m_uRow;
		int nSrcColumn = m_uColumn;
		int nDesRow = nSrcRow * resolution;
		int nDesColumn = nSrcColumn * resolution;
		_out_m = CMatrix<T>(nDesRow, nDesColumn);
		for (int r = 0; r < nSrcRow; r++)
		{
			for (int c = 0; c < nSrcColumn; c++)
			{
				for (int i = 0; i < resolution; i++)
				{
					for (int j = 0; j < resolution; j++)
					{
						MATmITEM(_out_m, r * resolution + i, c * resolution + j) = MAT_ITEM(r, c);
					}
				}
			}
		}
	}

public:	// Print
//	void ConsolePrint(char* fmt = NULL) const
//	{
//		CToStr to_str(MAT_ITEM(0, 0));
//		if (fmt)
//			to_str.m_fmt = fmt;
//		CStringA str;
//		for (int r = 0; r < m_uRow; r++)
//		{
//			for (int c = 0; c < m_uColumn; c++)
//			{
//				OutputDebugStringA(to_str(MAT_ITEM(r, c)));
//			}
//			OutputDebugStringA("\n");
//		}
//	}

};
