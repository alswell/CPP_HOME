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
	img = new unsigned char[cinfo.output_width * cinfo.output_height * unsigned(cinfo.output_components)];
	w = cinfo.output_width;
	h = cinfo.output_height;

	JSAMPROW row_pointer;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		row_pointer = &img[(cinfo.output_scanline) * cinfo.output_width * unsigned(cinfo.output_components)];
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

int jpeg2rgb(const char* filename, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom)
{
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	FILE* f = fopen(filename, "rb");
	jpeg_stdio_src(&cinfo, f);
	decompress(cinfo, img, w, h, scale_denom);
	fclose(f);

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	return 0;
}

void rgb2jpeg(const char* filename, unsigned char* img, unsigned w, unsigned h)
{
	FILE * f = fopen(filename, "wb");
	if (f == nullptr)
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

bool png2rgb(const char* filename, unsigned char*& img, unsigned& w, unsigned& h)
{
	FILE* f = fopen(filename, "rb");
	png_byte check_header[8];
	fread(check_header, 1, 8, f);
	if (png_sig_cmp(check_header, 0, 8) != 0)
	{
		fclose(f);
		return false;
	}
	auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	auto info_ptr = png_create_info_struct(png_ptr);
	//setjmp(png_jmpbuf(png_ptr));
	rewind(f);
	png_init_io(png_ptr, f);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR|PNG_TRANSFORM_EXPAND, nullptr);
	unsigned channels = png_get_channels(png_ptr, info_ptr);
	auto row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	//int color_type = png_get_color_type(png_ptr, info_ptr);
	//int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	w = png_get_image_width(png_ptr, info_ptr);
	h = png_get_image_height(png_ptr, info_ptr);
	img = new unsigned char[w * h * channels];
	png_bytepp rows = png_get_rows(png_ptr, info_ptr);

	unsigned char* p = img;
	for (unsigned i = 0; i < h; ++i, p += row_bytes)
		memcpy(p, rows[i], row_bytes);
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	fclose(f);
	return true;
}

void rgb2png(const char* filename, unsigned char* img, unsigned w, unsigned h)
{
	FILE* f = fopen(filename, "wb");
	auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	auto info_ptr = png_create_info_struct(png_ptr);
	png_init_io(png_ptr, f);
	png_set_IHDR(png_ptr, info_ptr, w, h, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);
	auto rows = new png_bytep[h];
	unsigned char* p = img;
	auto row_bytes = 4 * w;
	for (unsigned i = 0; i < h; ++i, p += row_bytes)
		rows[i] = p;
	png_set_rows(png_ptr, info_ptr, rows);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, nullptr);
	png_write_end(png_ptr, nullptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	delete[] rows;
	fclose(f);
}

