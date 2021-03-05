#include "JPEG.h"
#include "setjmp.h"

bool LoadJPG(const char* filename, CRGBImg& img, unsigned scale_denom)
{
	unsigned char* tmp;
	unsigned w;
	unsigned h;
	if (jpeg2rgb(filename, tmp, w, h, scale_denom) != 0)
		return false;
	img.Attach(h, w, reinterpret_cast<PIX32*>(tmp));
	return true;
}

bool LoadJPG(unsigned char* buff, unsigned size, CRGBImg& img, unsigned scale_denom)
{
	if (jpeg2rgb(buff, size, (unsigned char*&)img.m_pT, img.m_uColumn, img.m_uRow, 1) != 0)
		return false;
	return true;
}

class CJPEGCtxt
{
	jpeg_error_mgr m_JPEGErrMgr; // be sure this is the first member
	jmp_buf m_JmpBuf;
	jpeg_decompress_struct m_info;

	J_COLOR_SPACE m_enClrSpace;
	FILE* m_pFile;
public:
	CJPEGCtxt(J_COLOR_SPACE color_space)
	{
		m_pFile = nullptr;
		m_info.err = jpeg_std_error(&m_JPEGErrMgr);
		m_enClrSpace = color_space;
		m_JPEGErrMgr.error_exit = ErrHandler;
		jpeg_create_decompress(&m_info);
	}
	~CJPEGCtxt()
	{
		jpeg_destroy_decompress(&m_info);
		if (m_pFile)
			fclose(m_pFile);
	}
	void Load(unsigned char* buff, unsigned size)
	{
		jpeg_mem_src(&m_info, buff, size);
	}
	void Load(const char* filename)
	{
		m_pFile = fopen(filename, "rb");
		jpeg_stdio_src(&m_info, m_pFile);
	}
	int Decompress(unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom)
	{
		unsigned char* buf = nullptr;
		if (setjmp(m_JmpBuf))
		{
			if (buf)
				delete []buf;
			return -1;
		}

		jpeg_read_header(&m_info, true);

		m_info.scale_num = 1;
		m_info.scale_denom = scale_denom;
		m_info.out_color_space = m_enClrSpace;
		jpeg_start_decompress(&m_info);
		printf("%d * %d [%d] >> %d * %d [%d]\n", m_info.image_width, m_info.image_height, m_info.num_components,
			   m_info.output_width, m_info.output_height, m_info.output_components);
		if (img && m_info.output_width * m_info.output_height > w * h)
			return m_info.output_width * m_info.output_height;
		if (img == nullptr)
			buf = img = new unsigned char[m_info.output_width * m_info.output_height * unsigned(m_info.output_components)];
		w = m_info.output_width;
		h = m_info.output_height;

		JSAMPROW row_pointer;
		while (m_info.output_scanline < m_info.output_height)
		{
			row_pointer = &img[(m_info.output_scanline) * m_info.output_width * unsigned(m_info.output_components)];
			jpeg_read_scanlines(&m_info, &row_pointer, 1);
		}
		jpeg_finish_decompress(&m_info);
		return 0;
	}
	METHODDEF(void) ErrHandler(j_common_ptr p) __attribute__((noreturn))
	{
		auto ctxt = reinterpret_cast<CJPEGCtxt*>(p->err);
		(*p->err->output_message)(p);
		longjmp(ctxt->m_JmpBuf, 1);
	}
};

int jpeg2rgb(unsigned char* buff, unsigned size, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom)
{
	CJPEGCtxt ctxt(JCS_EXT_BGRA);
	ctxt.Load(buff, size);
	return ctxt.Decompress(img, w, h, scale_denom);
}

int jpeg2rgb(const char* filename, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom)
{
	CJPEGCtxt ctxt(JCS_EXT_BGRA);
	ctxt.Load(filename);
	return ctxt.Decompress(img, w, h, scale_denom);
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

