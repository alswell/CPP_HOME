#include "PNG.h"

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

