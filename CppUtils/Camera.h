#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <getopt.h>             /* getopt_long() */
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include <sys/types.h>
#include <string>
#include <vector>


#define CLEAR(x) memset (&(x), 0, sizeof (x))

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
	SFmtInfo(__u32 _fmt, char _name[])
	{
		fmt = _fmt;
		strcpy(name, _name);
	}
};

class Camera;
class CBuff
{
	Camera* m_pCam;
	v4l2_buffer m_buf;
public:
	void* start;
	size_t len;
	CBuff(Camera* pCam);
	~CBuff();
};

class Camera
{
	friend class CBuff;
public:
	Camera(const char *pDevName, bool print_detail = true);
	~Camera();
	bool Init(int w, int h, int nDataType = 0);
	int GetBuffer(void* image);
	CBuff* GetBuffer();

	unsigned GetImageSize();
	void Identify();
	void list_info(bool print_detail);
private:
	std::string m_strDevName;
	int m_fd;
	int m_nWidth;
	int m_nHeight;
	int m_nDataType;
	unsigned m_nImageSize;	//to keep the real image size!

	fd_set m_fds;
	int m_fdMax;
	struct timeval m_tmSelect; 	// timeout for select

	io_method m_ioMethod;
	std::vector<SFmtInfo> m_vFmts;
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
	CBuff* read_frame();

	void print_info(const char * str);
	void print_error(const char * str);
	void errno_exit(const char * str);
	int xioctl(int fd, int request, void * arg);

	bool DQBUF(v4l2_buffer& buf);
	bool QBUF(v4l2_buffer& buf);
};

#endif // CAMERA_H

