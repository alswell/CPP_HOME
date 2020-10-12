#pragma once
#include "ZoomFigure.h"

class CZoomKLine : public CZoomFigure
{
public:
	PointF m_posRT;
	void Add(float begin, float end, float low, float high);
};
