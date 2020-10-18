#pragma once

#include "../env.h"
#include "png.h"

bool png2rgb(const char* filename, unsigned char*& img, unsigned& w, unsigned& h);
void rgb2png(const char* filename, unsigned char* img, unsigned w, unsigned h);
