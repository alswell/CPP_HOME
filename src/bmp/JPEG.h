#pragma once

#include "../env.h"
#include "RGBImg.h"

#include "jpeglib.h"
#include "jerror.h"


bool LoadJPG(unsigned char* buff, unsigned size, CRGBImg& img);

int jpeg2rgb(unsigned char* buff, unsigned size, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom = 1);
int jpeg2rgb(const char* filename, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom = 1);
void rgb2jpeg(const char* filename, unsigned char* img, unsigned w, unsigned h);

