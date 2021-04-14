#pragma once

#include "../env.h"
#include "RGBImg.h"

#include "jpeglib.h"
#include "jerror.h"


bool LoadJPG(const char* filename, CRGBImg& img, unsigned scale_denom = 1);
bool LoadJPG(unsigned char* buff, unsigned size, CRGBImg& img, unsigned scale_denom = 1);

int JPEG2BGRA(unsigned char* buff, unsigned size, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom = 1);
int JPEG2BGRA(const char* filename, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom = 1);
int JPEG2RGBA(unsigned char* buff, unsigned size, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom = 1);
int JPEG2RGBA(const char* filename, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom = 1);

void BGRA2JPEG(const char* filename, unsigned char* img, unsigned w, unsigned h);
void RGBA2JPEG(const char* filename, unsigned char* img, unsigned w, unsigned h);
