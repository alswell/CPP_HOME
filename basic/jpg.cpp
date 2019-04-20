#include "jpg.h"

void decompress(jpeg_decompress_struct& cinfo, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom)
{
	jpeg_read_header(&cinfo, TRUE);

	cinfo.scale_num = 1;
	cinfo.scale_denom = scale_denom;
	cinfo.out_color_space = JCS_EXT_BGRA;
	jpeg_start_decompress(&cinfo);
	printf("%d * %d [%d] >> %d * %d [%d]\n", cinfo.image_width, cinfo.image_height, cinfo.num_components,
		   cinfo.output_width, cinfo.output_height, cinfo.output_components);
	img = new unsigned char[cinfo.output_width * cinfo.output_height * cinfo.output_components];
	w = cinfo.output_width;
	h = cinfo.output_height;

	JSAMPROW row_pointer;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		row_pointer = &img[(cinfo.output_scanline) * cinfo.output_width * cinfo.output_components];
		jpeg_read_scanlines(&cinfo, &row_pointer, 1);
	}
}

int jpeg2rgb(unsigned char* buff, unsigned size, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom)
{
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_mem_src(&cinfo, buff, size);
	decompress(cinfo, img, w, h, scale_denom);

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	return 0;
}

int jpeg2rgb(const char* file_name, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom)
{
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE* f = fopen(file_name, "rb");
	jpeg_stdio_src(&cinfo, f);
	decompress(cinfo, img, w, h, scale_denom);
	fclose(f);

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	return 0;
}

void rgb2jpeg(const char* file_name, unsigned char* img, unsigned w, unsigned h)
{
	FILE * f = fopen(file_name, "wb");
	if (f == NULL)
		return;

	jpeg_compress_struct cinfo;
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	jpeg_stdio_dest(&cinfo, f);
	cinfo.image_width = w;
	cinfo.image_height = h;
	cinfo.input_components = 4;
	cinfo.in_color_space = JCS_EXT_BGRA;
	jpeg_set_defaults(&cinfo);
	//jpeg_set_quality(&cinfo, 90, TRUE); // 压缩比为90%
	jpeg_start_compress(&cinfo, TRUE);
	JSAMPROW row_pointer;
	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer = &img[cinfo.next_scanline * cinfo.image_width * 4];
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(f);
}
