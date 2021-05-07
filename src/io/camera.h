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


#define CLEAR(x) memset (&(x), 0, sizeof (x))


class Camera
{
	friend class Frame;
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
	void CheckDev();
public:
	Camera(int nDevNum);
	Camera(const char* dev);
	~Camera();
	void PrintDevInfo();

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
	void SetMemBufCount(unsigned n);
	bool Init(unsigned w, unsigned h, unsigned nDataType = 0);
	void Restart();
	unsigned GetImage(void* image);
	Camera *GetFrame();
	unsigned GetW();
	unsigned GetH();

	unsigned GetImageSize();
	void Identify();
	void RetrievePixFmt();

	int AutoFocus(int bOn);
	int IsAutoFocus();
	int SetFocus(int nValue);
	int GetFocus();
private:
	CString m_strDevName;
	int m_fd;
	unsigned m_nWidth;
	unsigned m_nHeight;
	unsigned m_nDataType;
	unsigned m_nImageSize;	//to keep the real image size!
	unsigned m_nMemCount;
	SBuff * m_pBuff;

	fd_set m_fds;
	int m_fdMax;
	struct timeval m_tmSelect; 	// timeout for select

	v4l2_memory m_ioMethod;
	vector<SFmtInfo> m_vFmts;

	bool open_device(void);
	void close_device(void);
	int enum_frame_intervals(int dev, __u32 pixfmt, __u32 width, __u32 height);
	int enum_frame_sizes(int dev, __u32 pixfmt);

	bool check_device(void);
	bool init_device(void);
	bool init_mmap(void);
	bool init_user_prt(void);
	void uninit_device(void);
	bool m_bInited;

	bool start_capturing(void);
	void stop_capturing(void);
	bool m_bStarted;

	bool wait_frame();
	unsigned read_frame(void* image);

	void print_info(const char * str);
	void print_error(const char * str);
	void errno_exit(const char * str) __attribute__((noreturn));
	int xioctl(int fd, unsigned request, void * arg);

	bool DQBUF(v4l2_buffer& buf);
	bool QBUF(v4l2_buffer& buf);
};

