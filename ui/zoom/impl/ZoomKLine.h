#pragma once
#include "ZoomFigure.h"

class CZoomKLine : public CZoomFigure
{
public:
	void Add(float begin, float end, float low, float high);
};
