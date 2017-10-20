#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "camera.h"
#include "tick.h"

#define NUM_FRAM		100

int main(int argc, char ** argv) {
	if (argc == 1)
	{
		printf("usage: ./camera /dev/video? [w h] [nFmt]\n");
		return 0;
	}
	Camera *camera = new Camera(atoi(argv[1]), argc==2);

	if (argc == 2)
		return 0;

	int width = atoi(argv[2]);
	int height = atoi(argv[3]);
	int nFmt = 0;
	if (argc >= 5)
		nFmt = atoi(argv[4]);
	printf("capture frame size: %d * %d\n", width, height);
	camera->Init(width, height, nFmt);

	unsigned image_size = camera->GetImageSize();
	char* image = new char[image_size];
	printf("image_size: %d\n", image_size);
	
	unsigned int writesize = 0;
	GetTickCount();
	for (int i = 0; i < NUM_FRAM; i++) 
	{
		//image_size = camera->GetBuffer(image);
		//if (image_size == 0)
		//	break;
		CBuff* pBuff;
		pBuff = camera->GetBuffer();
		char fname[128];
		sprintf(fname, "img/camera-image%03d", i);
		FILE * outf = fopen(fname, "wb");
		//writesize = fwrite(image, 1, image_size, outf);
		writesize = fwrite(pBuff->start, 1, pBuff->len, outf);
		fflush(outf);
		fclose(outf);

		delete pBuff;
		printf("frame: %d, write size: %d\n", i, writesize);
		usleep(10000);
	}

	float totaltime2 = GetTickCount();
	totaltime2 /= 1000000;
	printf("time: %f, rate: %f\n", totaltime2, NUM_FRAM / totaltime2);
	return 0;
}

