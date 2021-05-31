#include "camera.h"



Camera::Camera(int nDevNum)
{
	m_strDevName.Format("/dev/video%d", nDevNum);
	PrepareDev();
}

Camera::Camera(const char *dev)
{
	m_strDevName.Format("/dev/v4l/by-path/%s", dev);
	PrepareDev();
}

Camera::~Camera()
{
	stop_capturing();
	uninit_device();
	if (-1 == close(m_fd))
		errno_exit("close dev failed");
	m_fd = -1;
}

void Camera::UseMMAP()
{
	m_ioMethod = V4L2_MEMORY_MMAP;
}

bool Camera::UseMJPG()
{
	for (unsigned i = 0; m_vFmts.size(); ++i) {
		if (m_vFmts[i].fmt == V4L2_PIX_FMT_MJPEG) {
			m_nDataFmt = V4L2_PIX_FMT_MJPEG;
			return true;
		}
	}
	return false;
}

void Camera::SetMemBufCount(unsigned n)
{
	m_nMemCount = n;
}

bool Camera::Init(unsigned w, unsigned h)
{
	check_device();

	m_nWidth = w;
	m_nHeight = h;
	if (!init_device())
		errno_exit("camera init failed");
	cout << "camera init success" << endl;

	if (!start_capturing())
		errno_exit("start_capturing failed");
	cout << "start_capturing success" << endl;

	return true;
}

unsigned Camera::GetImage(void* image)
{
	struct v4l2_buffer buf;
	DQBUF(buf);
	assert(buf.index < m_nMemCount);
	memcpy(image, m_pBuff[buf.index].start, buf.bytesused);
	unsigned bytesused = buf.bytesused;
	QBUF(buf);
	return bytesused;
}

Camera * Camera::GetFrame()
{
	return this;
}

unsigned Camera::GetW()
{
	return m_nWidth;
}

unsigned Camera::GetH()
{
	return m_nHeight;
}

unsigned Camera::GetImageSize()
{
	return m_nImageSize;
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

void Camera::PrintDevInfo()
{
	printf("Supported Format:\n");
	for (unsigned i = 0; i < m_vFmts.size(); ++i)
	{
		const char* p = (const char*)&m_vFmts[i].fmt;
		printf("\t[%d: %c%c%c%c] %s\n", i, p[0], p[1], p[2], p[3], m_vFmts[i].name);
		enum_frame_sizes(m_fd, m_vFmts[i].fmt);
		printf("\n");
	}
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
	printf("\t  time interval between frame:");
	while ((ret = ioctl(dev, VIDIOC_ENUM_FRAMEINTERVALS, &fival)) == 0) {
		if (fival.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
			printf("%u/%u, ",
				fival.discrete.numerator, fival.discrete.denominator);
		}
		else if (fival.type == V4L2_FRMIVAL_TYPE_CONTINUOUS) {
			printf("{min{ %u/%u } max{ %u/%u }}, ",
				fival.stepwise.min.numerator, fival.stepwise.min.numerator,
				fival.stepwise.max.denominator, fival.stepwise.max.denominator);
			break;
		}
		else if (fival.type == V4L2_FRMIVAL_TYPE_STEPWISE) {
			printf("{min{ %u/%u } max{ %u/%u } stepsize{ %u/%u }}, ",
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
		if (fsize.type == V4L2_FRMSIZE_TYPE_CONTINUOUS)
		{
			printf("\t{ CONTINUOUS: min{ %u * %u } max{ %u * %u } }\n",
				fsize.stepwise.min_width, fsize.stepwise.min_height,
				fsize.stepwise.max_width, fsize.stepwise.max_height);
			printf("\tRefusing to enumerate frame intervals.\n");
			break;
		}
		if (fsize.type == V4L2_FRMSIZE_TYPE_STEPWISE)
		{
			printf("\t{ STEPWISE: min{ %u * %u } max { %u * %u } stepsize{ %u * %u } }\n",
				fsize.stepwise.min_width, fsize.stepwise.min_height,
				fsize.stepwise.max_width, fsize.stepwise.max_height,
				fsize.stepwise.step_width, fsize.stepwise.step_height);
			printf("\tRefusing to enumerate frame intervals.\n");
			break;
		}
		if (fsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
			printf("\t- %u * %u\n", fsize.discrete.width, fsize.discrete.height);
			ret = enum_frame_intervals(dev, pixfmt, fsize.discrete.width, fsize.discrete.height);
			if (ret != 0)
				printf("\tUnable to enumerate frame sizes.\n");
		}
		fsize.index++;
	}
	if (ret != 0 && errno != EINVAL) {
		perror("ERROR enumerating frame sizes");
		return errno;
	}

	return 0;
}

void Camera::PrepareDev()
{
	m_ioMethod = V4L2_MEMORY_USERPTR;
	m_nDataFmt = V4L2_PIX_FMT_MJPEG;
	m_nImageSize = 0;
	m_nMemCount = 4;

	struct stat st;
	if (-1 == stat(m_strDevName, &st))
		errno_exit("stat fail!!!");
	if (!S_ISCHR(st.st_mode))
		errno_exit("not char dev!!!");
	m_fd = open(m_strDevName, O_RDWR /*| O_NONBLOCK*/, 0);
	if (-1 == m_fd)
		errno_exit("open dev failed");
	cout << "open camera success\n" << endl;

	struct v4l2_fmtdesc fmtdesc;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	for (fmtdesc.index = 0; ioctl(m_fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1; ++fmtdesc.index)
		m_vFmts.push_back(SFmtInfo(fmtdesc.pixelformat, reinterpret_cast<char*>(fmtdesc.description)));
}

bool Camera::check_device(void)
{
	struct v4l2_capability cap;
//	struct v4l2_cropcap cropcap;
//	struct v4l2_crop crop;
	if (-1 == xioctl(m_fd, VIDIOC_QUERYCAP, &cap))
		errno_exit("VIDIOC_QUERYCAP: not V4L2 dev");

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
		errno_exit("not video capture device");

	switch (m_ioMethod)
	{
//	case IO_METHOD_READ:
//		if (!(cap.capabilities & V4L2_CAP_READWRITE))
//			errno_exit("not support read i/o");
//		break;
	case V4L2_MEMORY_MMAP:
	case V4L2_MEMORY_USERPTR:
		if (!(cap.capabilities & V4L2_CAP_STREAMING))
			errno_exit("not support streaming i/o");
		break;
	default:
		errno_exit("unsupported m_ioMethod");
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
	fmt.fmt.pix.pixelformat = m_nDataFmt; //V4L2_PIX_FMT_NV12;//V4L2_PIX_FMT_YUV420;//V4L2_PIX_FMT_MJPEG;//
	//fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if (-1 == xioctl(m_fd, VIDIOC_S_FMT, &fmt))
		errno_exit("VIDIOC_S_FMT fail");

	/* Note VIDIOC_S_FMT may change width and height. */
	if (-1 == xioctl(m_fd, VIDIOC_G_FMT, &fmt))
		errno_exit("VIDIOC_G_FMT fail");

	printf("after set fmt:\n");
	printf("\tfmt.fmt.pix.width = %d\n", fmt.fmt.pix.width);
	printf("\tfmt.fmt.pix.height = %d\n", fmt.fmt.pix.height);
	printf("\tfmt.fmt.pix.sizeimage = %d\n", fmt.fmt.pix.sizeimage);
	printf("\tfmt.fmt.pix.bytesperline = %d\n", fmt.fmt.pix.bytesperline);
	m_nImageSize = fmt.fmt.pix.sizeimage;

//	printf("-#-#-#-#-#-#-#-#-#-#-#-#-#-\n");
//	/* Buggy driver paranoia. */
//	unsigned min = fmt.fmt.pix.width * 2;
//	if (fmt.fmt.pix.bytesperline < min)
//		fmt.fmt.pix.bytesperline = min;
//	min = m_nWidth * m_nHeight * 3 / 2;
//	printf("min:%d\n", min);
//	if (fmt.fmt.pix.sizeimage < min)
//		fmt.fmt.pix.sizeimage = min;
//	printf("After Buggy driver paranoia\n");
//	printf("\tfmt.fmt.pix.sizeimage = %d\n", fmt.fmt.pix.sizeimage);
//	printf("\tfmt.fmt.pix.bytesperline = %d\n", fmt.fmt.pix.bytesperline);
//	printf("-#-#-#-#-#-#-#-#-#-#-#-#-#-\n\n");
//	m_nImageSize = fmt.fmt.pix.sizeimage;

	if (m_ioMethod == V4L2_MEMORY_MMAP)
		init_mmap();
	else
		init_user_prt();
	return true;
}

bool Camera::init_mmap(void) 
{
	struct v4l2_requestbuffers req;
	CLEAR(req);
	req.count = m_nMemCount;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	if (-1 == xioctl(m_fd, VIDIOC_REQBUFS, &req))
	{
		if (EINVAL == errno)
			errno_exit("VIDIOC_REQBUFS: dev does not support memory mapping");
		else
			errno_exit("VIDIOC_REQBUFS: mmap");
	}
	//printf("req.capabilities: 0x%X, req.count: %d\n", req.capabilities, req.count);
	printf("req.count: %d\n", req.count);
	m_nMemCount = req.count;
	m_pBuff = new SBuff[req.count];
	for (unsigned i = 0; i < req.count; ++i)
	{
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if (-1 == xioctl(m_fd, VIDIOC_QUERYBUF, &buf))
			errno_exit("VIDIOC_QUERYBUF");
		m_pBuff[i].length = buf.length;
		m_pBuff[i].start = mmap(nullptr, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, buf.m.offset);
		if (MAP_FAILED == m_pBuff[i].start)
			return false;
	}
	return true;
}

bool Camera::init_user_prt()
{
	struct v4l2_requestbuffers req;
	CLEAR(req);
	req.count = m_nMemCount;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_USERPTR;
	if (-1 == xioctl(m_fd, VIDIOC_REQBUFS, &req))
	{
		if (EINVAL == errno)
			errno_exit("VIDIOC_REQBUFS: dev does not support user ptr");
		else
			errno_exit("VIDIOC_REQBUFS: uptr");
	}
	//printf("req.capabilities: 0x%X, req.count: %d\n", req.capabilities, req.count);
	printf("req.count: %d\n", req.count);
	m_nMemCount = req.count;
	m_pBuff = new SBuff[req.count];
	for (unsigned i = 0; i < req.count; ++i)
	{
		m_pBuff[i].length = m_nImageSize;//1024*1024;
		m_pBuff[i].start = malloc(m_pBuff[i].length);
	}
	return true;
}

void Camera::uninit_device(void) 
{
	switch (m_ioMethod)
	{
//	case IO_METHOD_READ:
//		//free(m_pBuff[0].start);
//		break;
	case V4L2_MEMORY_MMAP:
		for (unsigned i = 0; i < m_nMemCount; ++i)
			if (-1 == munmap(m_pBuff[i].start, m_pBuff[i].length))
				errno_exit("munmap");
		break;
	case V4L2_MEMORY_USERPTR:
		for (unsigned i = 0; i < m_nMemCount; ++i)
			free(m_pBuff[i].start);
		break;
	default:
		break;
	}
	//free(m_pBuff);
	delete[] m_pBuff;
}

bool Camera::start_capturing(void)
{
	for (unsigned i = 0; i < m_nMemCount; ++i)
	{
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = m_ioMethod;
		buf.index = i;
		if (m_ioMethod == V4L2_MEMORY_USERPTR)
		{
			buf.length = m_pBuff[i].length;
			buf.m.userptr = (unsigned long)m_pBuff[i].start;
		}
		if (-1 == xioctl(m_fd, VIDIOC_QBUF, &buf))
			errno_exit("start_capturing: VIDIOC_QBUF");
	}
	v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == xioctl(m_fd, VIDIOC_STREAMON, &type))
		errno_exit("start_capturing: VIDIOC_STREAMON");

	return true;
}

void Camera::stop_capturing(void)
{
	enum v4l2_buf_type type;
	switch (m_ioMethod)
	{
//	case IO_METHOD_READ:
//		/* Nothing to do. */
//		break;
	case V4L2_MEMORY_MMAP:
	case V4L2_MEMORY_USERPTR:
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (-1 == xioctl(m_fd, VIDIOC_STREAMOFF, &type))
			errno_exit("VIDIOC_STREAMOFF");
		break;
	default:
		break;
	}
}

void Camera::errno_exit(const char * str)
{
	//fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
	printf("[%s] %s ?_? %m(%d)\n", m_strDevName.GetString(), str, errno);
	exit(EXIT_FAILURE);
}

int Camera::xioctl(int fd, unsigned request, void * arg)
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
	buf.index = -1;
	buf.bytesused = -1;
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = m_ioMethod;
	if (-1 == xioctl(m_fd, VIDIOC_DQBUF, &buf))
	{
		switch (errno)
		{
		case EAGAIN:
			printf("Camera::DQBUF Error: AGAIN [%d]bytesused: %d\n", buf.index, buf.bytesused);
			return false;
		case EIO:
			printf("Camera::DQBUF Error: IO [%d]bytesused: %d\n", buf.index, buf.bytesused);
			/* Could ignore EIO, see spec. */
			[[clang::fallthrough]];
		default:
			errno_exit("Camera: VIDIOC_DQBUF");
		}
	}
	printf("buf.index: %d; buf.bytesused: %d\n", buf.index, buf.bytesused);

	return true;
}

bool Camera::QBUF(v4l2_buffer & buf)
{
	if (buf.index == unsigned(-1))
		return false;
	printf("buf.index: %d; buf.bytesused: %d VIDIOC_QBUF\n", buf.index, buf.bytesused);
	if (-1 == xioctl(m_fd, VIDIOC_QBUF, &buf))
		errno_exit("Camera: VIDIOC_QBUF");
	return true;
}


Camera::SFmtInfo::SFmtInfo(__u32 _fmt, char _name[])
{
	fmt = _fmt;
	strcpy(name, _name);
}

#include "datetime.h"
Camera::Frame::Frame(Camera* pCam)
	: m_pCam(pCam)
	, start(nullptr)
	, len(0)
{
	CTickHelper h("get frame");
	if (m_pCam && m_pCam->DQBUF(m_buf))
	{
		start = m_pCam->m_pBuff[m_buf.index].start;
		len = m_buf.bytesused;
	}
}

Camera::Frame::~Frame()
{
	m_pCam->QBUF(m_buf);
}
