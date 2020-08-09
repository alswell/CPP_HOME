#pragma once
#include "cycl.h"
#include "direction.h"

extern int g_OrderX[8];
extern int g_OrderY[8];

class C8Neighbor
{
public:
	class S
	{
	public:
		CVector2<int> pt;
		CDirection<float> dir;
		S() {}
		S(int x, int y) : pt(x, y), dir(0, x, y) {}
		bool operator == (const S& s) const
		{
			return pt == s.pt;
		}
	};
	cycl<S> m_Order;
	C8Neighbor()
	{
		m_Order.insert_next(S(-1, -1));
		m_Order.insert_next(S(-1, 0));
		m_Order.insert_next(S(-1, 1));
		m_Order.insert_next(S(0, 1));
		m_Order.insert_next(S(1, 1));
		m_Order.insert_next(S(1, 0));
		m_Order.insert_next(S(1, -1));
		m_Order.insert_next(S(0, -1));
	}
	void ResetEntry()
	{
		m_Order.set_entry(S(-1, -1));
	}
};
extern C8Neighbor g_clOrder;
