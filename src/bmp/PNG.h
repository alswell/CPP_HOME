#pragma once

#include "../env.h"
#include "RGBImg.h"
#include "png.h"

void LoadPNG(const char* filename, CRGBImg& img);

bool png2rgb(const char* filename, unsigned char*& img, unsigned& w, unsigned& h);
void rgb2png(const char* filename, unsigned char* img, unsigned w, unsigned h);
