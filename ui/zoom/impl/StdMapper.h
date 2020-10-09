#pragma once
#include "../IZoom.h"

class CStdMapper : public IBmpMapper
{
public:
	virtual void Map(float& x, float& y);
	virtual void Revert(float& x, float& y);
};
