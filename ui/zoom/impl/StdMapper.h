#pragma once
#include "zoom/IZoom.h"

class CStdMapper : public IBmpMapper
{
public:
	virtual void Map(float& x, float& y);
	virtual void Revert(float& x, float& y);
};
