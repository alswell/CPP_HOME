#pragma once

#include "env.h"
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>              /* low-level i/o */
#include <getopt.h>             /* getopt_long() */
#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include "str.h"


#define CLEAR(x) memset (&(x), 0, sizeof (x))


class Camera
{
	friend class Frame;
	typedef enum
	{
		IO_METHOD_READ,
		IO_METHOD_MMAP,
		IO_METHOD_USERPTR,
	} io_method;

	struct SBuff
	{
		void * start;
		size_t length;	//buffer's length is different from cap_image_size
	};
	struct SFmtInfo
	{
		__u32 fmt;
		char name[32];
		SFmtInfo(__u32 _fmt, char _name[]);
	};
public:
	Camera(int nDevNum, bool print_detail = true);
	~Camera();

	class Frame
	{
		Camera* m_pCam;
		v4l2_buffer m_buf;
	public:
		void* start;
		size_t len;
		Frame(Camera* pCam);
		~Frame();
	};

	bool Init(int w, int h, int nDataType = 0);
	int GetImage(void* image);
	Camera *GetFrame();

	unsigned GetImageSize();
	void Identify();
	void list_info(bool print_detail);

	int AutoFocus(int bOn);
	int SetFocus(int nValue);
	int GetFocus();
private:
	CString m_strDevName;
	int m_fd;
	int m_nWidth;
	int m_nHeight;
	int m_nDataType;
	unsigned m_nImageSize;	//to keep the real image size!

	fd_set m_fds;
	int m_fdMax;
	struct timeval m_tmSelect; 	// timeout for select

	io_method m_ioMethod;
    vector<SFmtInfo> m_vFmts;
	SBuff * m_pBuff;
	unsigned m_nBuff;

	bool open_device(void);
	void close_device(void);
	int enum_frame_intervals(int dev, __u32 pixfmt, __u32 width, __u32 height);
	int enum_frame_sizes(int dev, __u32 pixfmt);

	bool check_device(void);
	bool init_device(void);
	bool init_mmap(void);
	void uninit_device(void);

	bool start_capturing(void);
	void stop_capturing(void);

	bool wait_frame();
	int read_frame(void* image);

	void print_info(const char * str);
	void print_error(const char * str);
	void errno_exit(const char * str);
	int xioctl(int fd, int request, void * arg);

	bool DQBUF(v4l2_buffer& buf);
	bool QBUF(v4l2_buffer& buf);
};

