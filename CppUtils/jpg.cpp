#include "jpg.h"

int jpeg2rgb(unsigned char* buff, unsigned size, unsigned char*& rdata, unsigned& w, unsigned& h)
{
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, buff, size);
	jpeg_read_header(&cinfo, TRUE);

	cinfo.out_color_space = JCS_EXT_BGRA;
	jpeg_start_decompress(&cinfo);
	printf("%d * %d [%d:%d]\n", cinfo.image_width, cinfo.image_height,
		   cinfo.num_components, cinfo.output_components);
	rdata = new unsigned char[cinfo.image_width * cinfo.image_height * cinfo.output_components];
	w = cinfo.image_width;
	h = cinfo.image_height;

	JSAMPROW row_pointer[1];
	while (cinfo.output_scanline < cinfo.output_height)
	{
		row_pointer[0] = &rdata[(cinfo.output_scanline) * cinfo.image_width * cinfo.output_components];
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	return 0;
}

