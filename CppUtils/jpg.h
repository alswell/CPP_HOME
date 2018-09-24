#pragma once

#include "env.h"

#include "jpeglib.h"
#include "jerror.h"

int jpeg2rgb(unsigned char* buff, unsigned size, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom = 1);
int jpeg2rgb(const char* file_name, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom = 1);

void rgb2jpeg(const char* file_name, unsigned char* img, unsigned w, unsigned h);
