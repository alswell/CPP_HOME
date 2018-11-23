#include "camera.h"


void Camera::CheckDev()
{
	m_ioMethod = IO_METHOD_MMAP;//IO_METHOD_READ;//IO_METHOD_MMAP;
	m_nImageSize = 0;
	m_nMemCount = 4;

	Identify();
	m_fd = -1;
	if (!open_device())
		errno_exit("open failed");
	printf("open success\n");
	check_device();
	RetrievePixFmt();

	m_fdMax = m_fd + 1;
	m_bInited = false;
	m_bStarted = false;
}

Camera::Camera(int nDevNum)
{
	m_strDevName.Format("/dev/video%d", nDevNum);
	CheckDev();
}

Camera::Camera(const char *dev)
{
	m_strDevName = dev;
	CheckDev();
}

Camera::~Camera()
{
	if (m_bStarted)
		stop_capturing();
	if (m_bInited)
		uninit_device();
	close_device();
}

void Camera::PrintDevInfo()
{
	printf("Support format:\n");
	for (int i = 0; i < m_vFmts.size(); ++i)
	{
		printf("\t%d. %s\n", i, m_vFmts[i].name);
		enum_frame_sizes(m_fd, m_vFmts[i].fmt);
	}
}

void Camera::SetMemBufCount(unsigned n)
{
	m_nMemCount = n;
}

unsigned Camera::GetImageSize() 
{
	return m_nImageSize;
}

bool Camera::Init(int w, int h, int nDataType)
{
	m_nWidth = w;
	m_nHeight = h;
	m_nDataType = nDataType;
	if (!init_device())
	{
		printf("init failed\n");
		return false;
	}
	m_bInited = true;
	printf("init success\n");

	if (!start_capturing())
	{
		printf("start_capturing failed\n");
		return false;
	}
	m_bStarted = true;
	printf("start_capturing success\n");

	return true;
}

int Camera::GetImage(void* image)
{
	if (wait_frame())
		return read_frame(image);
	return 0;
}

Camera * Camera::GetFrame()
{
	if (wait_frame())
		return this;
	return NULL;
}

void Camera::Identify()
{
	struct stat st;
	if (-1 == stat(m_strDevName, &st))
		errno_exit("stat fail!!!");
	
	if (!S_ISCHR(st.st_mode)) 
		errno_exit("not char dev!!!");
}

void Camera::RetrievePixFmt()
{
	struct v4l2_fmtdesc fmtdesc;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	for (fmtdesc.index = 0; ioctl(m_fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1; ++fmtdesc.index)
		m_vFmts.push_back(SFmtInfo(fmtdesc.pixelformat, (char*)fmtdesc.description));
}

bool Camera::open_device(void)
{
	m_fd = open(m_strDevName, O_RDWR /* required */ | O_NONBLOCK, 0);
	if (-1 == m_fd)
		errno_exit("open dev failed");

	return true;
}

void Camera::close_device(void)
{
	if (-1 == close(m_fd))
		errno_exit("close dev failed");
	m_fd = -1;
}

int Camera::AutoFocus(int bOn)
{
	struct v4l2_control control;
	control.id = V4L2_CID_FOCUS_AUTO;
	control.value = bOn;
	return ioctl(m_fd, VIDIOC_S_CTRL, &control);
}

int Camera::IsAutoFocus()
{
	struct v4l2_control control;
	control.id = V4L2_CID_FOCUS_AUTO;
	control.value = 0;
	int r = ioctl(m_fd, VIDIOC_G_CTRL, &control);
	return (r == -1) ? -1 : control.value;
}

int Camera::SetFocus(int nValue)
{
	struct v4l2_control control;
	control.id = V4L2_CID_FOCUS_ABSOLUTE;
	control.value = nValue;
	return ioctl(m_fd, VIDIOC_S_CTRL, &control);
}

int Camera::GetFocus()
{
	struct v4l2_control control;
	control.id = V4L2_CID_FOCUS_ABSOLUTE;
	control.value = 0;
	int r = ioctl(m_fd, VIDIOC_G_CTRL, &control);
	return (r == -1) ? -1 : control.value;
}

int Camera::enum_frame_intervals(int dev, __u32 pixfmt, __u32 width, __u32 height)
{
	int ret;
	struct v4l2_frmivalenum fival;

	memset(&fival, 0, sizeof(fival));
	fival.index = 0;
	fival.pixel_format = pixfmt;
	fival.width = width;
	fival.height = height;
	printf("\tTime interval between frame: ");
	while ((ret = ioctl(dev, VIDIOC_ENUM_FRAMEINTERVALS, &fival)) == 0) {
		if (fival.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
			printf("%u/%u, ",
				fival.discrete.numerator, fival.discrete.denominator);
		}
		else if (fival.type == V4L2_FRMIVAL_TYPE_CONTINUOUS) {
			printf("{min { %u/%u } .. max { %u/%u } }, ",
				fival.stepwise.min.numerator, fival.stepwise.min.numerator,
				fival.stepwise.max.denominator, fival.stepwise.max.denominator);
			break;
		}
		else if (fival.type == V4L2_FRMIVAL_TYPE_STEPWISE) {
			printf("{min { %u/%u } .. max { %u/%u } / "
				"stepsize { %u/%u } }, ",
				fival.stepwise.min.numerator, fival.stepwise.min.denominator,
				fival.stepwise.max.numerator, fival.stepwise.max.denominator,
				fival.stepwise.step.numerator, fival.stepwise.step.denominator);
			break;
		}
		fival.index++;
	}
	printf("\n");
	if (ret != 0 && errno != EINVAL) {
		perror("ERROR enumerating frame intervals");
		return errno;
	}

	return 0;
}

int Camera::enum_frame_sizes(int dev, __u32 pixfmt)
{
	int ret;
	struct v4l2_frmsizeenum fsize;

	memset(&fsize, 0, sizeof(fsize));
	fsize.index = 0;
	fsize.pixel_format = pixfmt;
	while ((ret = ioctl(dev, VIDIOC_ENUM_FRAMESIZES, &fsize)) == 0) {
		if (fsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
			printf("{ discrete: width = %u, height = %u }\n",
				fsize.discrete.width, fsize.discrete.height);
			ret = enum_frame_intervals(dev, pixfmt,
				fsize.discrete.width, fsize.discrete.height);
			if (ret != 0)
				printf("  Unable to enumerate frame sizes.\n");
		}
		else if (fsize.type == V4L2_FRMSIZE_TYPE_CONTINUOUS) {
			printf("{ continuous: min { width = %u, height = %u } .. "
				"max { width = %u, height = %u } }\n",
				fsize.stepwise.min_width, fsize.stepwise.min_height,
				fsize.stepwise.max_width, fsize.stepwise.max_height);
			printf("  Refusing to enumerate frame intervals.\n");
			break;
		}
		else if (fsize.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
			printf("{ stepwise: min { width = %u, height = %u } .. "
				"max { width = %u, height = %u } / "
				"stepsize { width = %u, height = %u } }\n",
				fsize.stepwise.min_width, fsize.stepwise.min_height,
				fsize.stepwise.max_width, fsize.stepwise.max_height,
				fsize.stepwise.step_width, fsize.stepwise.step_height);
			printf("  Refusing to enumerate frame intervals.\n");
			break;
		}
		fsize.index++;
	}
	if (ret != 0 && errno != EINVAL) {
		perror("ERROR enumerating frame sizes");
		return errno;
	}

	return 0;
}

bool Camera::check_device(void)
{
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	if (-1 == xioctl(m_fd, VIDIOC_QUERYCAP, &cap))
		errno_exit("VIDIOC_QUERYCAP: not V4L2 dev");

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
		errno_exit("not video capture device");

	switch (m_ioMethod)
	{
	case IO_METHOD_READ:
		if (!(cap.capabilities & V4L2_CAP_READWRITE))
			errno_exit("not support read i/o");
		break;
	case IO_METHOD_MMAP:
	case IO_METHOD_USERPTR:
		if (!(cap.capabilities & V4L2_CAP_STREAMING))
			errno_exit("not support streaming i/o");
		break;
	}
	return true;
}

bool Camera::init_device(void)
{
	//v4l2_input input;
	//memset(&input, 0, sizeof(struct v4l2_input));
	//input.index = 0;
	//int rtn = ioctl(fd, VIDIOC_S_INPUT, &input);
	//if (rtn < 0)
	//	errno_exit("VIDIOC_S_INPUT");

	struct v4l2_format fmt;
	CLEAR(fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//V4L2_BUF_TYPE_PRIVATE;//
	fmt.fmt.pix.width = m_nWidth;
	fmt.fmt.pix.height = m_nHeight;
	//V4L2_PIX_FMT_YUV420 : Planar formats with 1/2 horizontal and vertical chroma resolution, also known as YUV 4:2:0
	//V4L2_PIX_FMT_YUYV : Packed format with 1/2 horizontal chroma resolution, also known as YUV 4:2:2
	fmt.fmt.pix.pixelformat = m_vFmts[m_nDataType].fmt; //V4L2_PIX_FMT_NV12;//V4L2_PIX_FMT_YUV420;//V4L2_PIX_FMT_MJPEG;//
	printf("set image fmt to %d * %d [%s]\n", fmt.fmt.pix.width, fmt.fmt.pix.height, m_vFmts[m_nDataType].name);	
	//fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if (-1 == xioctl(m_fd, VIDIOC_S_FMT, &fmt))
		errno_exit("VIDIOC_S_FMT fail");

	if (-1 == xioctl(m_fd, VIDIOC_G_FMT, &fmt))
		errno_exit("VIDIOC_G_FMT fail");

	printf("after set fmt:\n");
	printf("\tfmt.fmt.pix.width = %d\n", fmt.fmt.pix.width);
	printf("\tfmt.fmt.pix.height = %d\n", fmt.fmt.pix.height);
	printf("\tfmt.fmt.pix.sizeimage = %d\n", fmt.fmt.pix.sizeimage);
	printf("\tfmt.fmt.pix.bytesperline = %d\n", fmt.fmt.pix.bytesperline);
	m_nImageSize = fmt.fmt.pix.sizeimage;

	/* Note VIDIOC_S_FMT may change width and height. */
	printf("-#-#-#-#-#-#-#-#-#-#-#-#-#-\n");
	/* Buggy driver paranoia. */
	int min = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = m_nWidth * m_nHeight * 3 / 2;
	printf("min:%d\n", min);
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;
	printf("After Buggy driver paranoia\n");
	printf("\tfmt.fmt.pix.sizeimage = %d\n", fmt.fmt.pix.sizeimage);
	printf("\tfmt.fmt.pix.bytesperline = %d\n", fmt.fmt.pix.bytesperline);
	printf("-#-#-#-#-#-#-#-#-#-#-#-#-#-\n\n");
	m_nImageSize = fmt.fmt.pix.sizeimage;

	init_mmap();
	return true;
}

bool Camera::init_mmap(void) 
{
	struct v4l2_requestbuffers req;
	CLEAR(req);
	req.count = m_nMemCount;//4;//2;//
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	if (-1 == xioctl(m_fd, VIDIOC_REQBUFS, &req))
	{
		if (EINVAL == errno)
			errno_exit("VIDIOC_REQBUFS: dev does not support memory mapping");
		else
			errno_exit("VIDIOC_REQBUFS: xxx");
	}
	printf("req.count: %d\n", req.count);
	//if (req.count < 2)
	//	errno_exit("insufficient buffer memory on dev");

	m_pBuff = new SBuff[req.count];

	for (m_nBuff = 0; m_nBuff < req.count; ++m_nBuff) 
	{
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = m_nBuff;
		if (-1 == xioctl(m_fd, VIDIOC_QUERYBUF, &buf))
			errno_exit("VIDIOC_QUERYBUF");
		m_pBuff[m_nBuff].length = buf.length;
		m_pBuff[m_nBuff].start = mmap(NULL /* start anywhere */, buf.length,
			PROT_READ | PROT_WRITE /* required */,
			MAP_SHARED /* recommended */, m_fd, buf.m.offset);
		if (MAP_FAILED == m_pBuff[m_nBuff].start)
			return false;
	}
	return true;
}

void Camera::uninit_device(void) 
{
	unsigned int i;
	switch (m_ioMethod)
	{
	case IO_METHOD_READ:
		//free(m_pBuff[0].start);
		break;
	case IO_METHOD_MMAP:
		for (i = 0; i < m_nBuff; ++i)
			if (-1 == munmap(m_pBuff[i].start, m_pBuff[i].length))
				errno_exit("munmap");
		break;
	case IO_METHOD_USERPTR:
		//for (i = 0; i < m_nBuff; ++i)
		//	free(m_pBuff[i].start);
		break;
	}
	//free(m_pBuff);
	delete[] m_pBuff;
}

bool Camera::start_capturing(void)
{
	unsigned int i;
	enum v4l2_buf_type type;
	for (i = 0; i < m_nBuff; ++i) 
	{
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if (-1 == xioctl(m_fd, VIDIOC_QBUF, &buf))
			errno_exit("start_capturing: VIDIOC_QBUF");
	}
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == xioctl(m_fd, VIDIOC_STREAMON, &type))
		errno_exit("start_capturing: VIDIOC_STREAMON");

	return true;
}

void Camera::stop_capturing(void)
{
	enum v4l2_buf_type type;
	switch (m_ioMethod)
	{
	case IO_METHOD_READ:
		/* Nothing to do. */
		break;
	case IO_METHOD_MMAP:
	case IO_METHOD_USERPTR:
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(m_fd, VIDIOC_STREAMOFF, &type))
			errno_exit("VIDIOC_STREAMOFF");
		break;
	}
}

bool Camera::wait_frame()
{
	FD_ZERO(&m_fds);
	FD_SET(m_fd, &m_fds);
	m_tmSelect.tv_sec = 2;
	m_tmSelect.tv_usec = 0;
	int r = select(m_fdMax, &m_fds, NULL, NULL, &m_tmSelect);
	printf("select time remain: %d.%06d\n", m_tmSelect.tv_sec, m_tmSelect.tv_usec);
	if (-1 == r)
		errno_exit("select");

	if (0 == r)
	{
		printf("select timeout\n");
		return false;
	}
	return true;
}

int Camera::read_frame(void* image)
{
	struct v4l2_buffer buf;
	DQBUF(buf);
	assert(buf.index < m_nBuff);
	memcpy(image, m_pBuff[buf.index].start, buf.bytesused);
	int bytesused = buf.bytesused;
	QBUF(buf);
	return bytesused;
}

void Camera::print_info(const char * str)
{
	printf("[%s] %s", (const char*)m_strDevName, str);
}

void Camera::print_error(const char * str)
{
	printf("[%s] %s ?_? %m(%d)\n", (const char*)m_strDevName, str, errno);
}

void Camera::errno_exit(const char * str)
{
	//fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
	print_error(str);
	exit(EXIT_FAILURE);
}

int Camera::xioctl(int fd, int request, void * arg)
{
	int r;
	do
		r = ioctl(fd, request, arg);
	while (-1 == r && EINTR == errno);
	return r;
}

bool Camera::DQBUF(v4l2_buffer& buf)
{
	CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	if (-1 == xioctl(m_fd, VIDIOC_DQBUF, &buf))
	{
		switch (errno)
		{
		case EAGAIN:
			printf("EAGAIN\n");
			return 0;
		case EIO:
			printf("EIO\n");
			/* Could ignore EIO, see spec. */
			/* fall through */
		default:
			errno_exit("VIDIOC_DQBUF");
		}
	}
	printf("buf.index: %d; buf.bytesused: %d\n", buf.index, buf.bytesused);

	return true;
}

bool Camera::QBUF(v4l2_buffer & buf)
{
	if (-1 == xioctl(m_fd, VIDIOC_QBUF, &buf))
		errno_exit("read_frame: VIDIOC_QBUF");
	return true;
}


Camera::SFmtInfo::SFmtInfo(__u32 _fmt, char _name[])
{
	fmt = _fmt;
	strcpy(name, _name);
}

Camera::Frame::Frame(Camera* pCam)
	: m_pCam(pCam)
{
	m_pCam->DQBUF(m_buf);
	start = m_pCam->m_pBuff[m_buf.index].start;
	len = m_buf.bytesused;
}

Camera::Frame::~Frame()
{
	m_pCam->QBUF(m_buf);
}
