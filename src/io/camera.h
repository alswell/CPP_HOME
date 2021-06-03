#pragma once

#include "../env.h"
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>              /* low-level i/o */
#include <getopt.h>             /* getopt_long() */
#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>
#include "../str.h"


class Camera
{
	friend class Frame;
	struct SBuff
	{
		void * start;
		size_t length;	//buffer's length is different from cap_image_size
	};
public:
	Camera(int nDevNum);
	Camera(const char* dev);
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

	void UseMMAP();
	bool SetPixFmt(unsigned format);
	bool SetPixFmtMJPG();
	bool SetPixFmtYUYV();
	void SetMemBufCount(unsigned n);
	bool Init(unsigned w, unsigned h);
	unsigned GetImage(void* image);

	int AutoFocus(int bOn);
	int IsAutoFocus();
	int SetFocus(int nValue);
	int GetFocus();

	void PrintDevInfo();
private:
	CString m_strDevName;
	int m_fd;
	SBuff * m_pBuff;

	struct v4l2_format fmt;
	struct v4l2_requestbuffers req;

	int enum_frame_intervals(int dev, __u32 pixfmt, __u32 width, __u32 height);
	int enum_frame_sizes(int dev, __u32 pixfmt);

	void PrepareDev();
	bool check_device(void);
	bool init_device(void);
	bool init_mmap(void);
	bool init_user_prt(void);
	void uninit_device(void);

	bool start_capturing(void);
	void stop_capturing(void);

	void errno_exit(const char * str) __attribute__((noreturn));
	int xioctl(int fd, unsigned request, void * arg);

	bool DQBUF(v4l2_buffer& buf);
	bool QBUF(v4l2_buffer& buf);
};

