#include "camera.h"

#define CLEAR(x) memset (&(x), 0, sizeof (x))

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
	if (m_pBuff)
	{
		stop_capturing();
		uninit_device();
	}
	if (-1 == close(m_fd))
		errno_exit("close dev failed");
	m_fd = -1;
}

void Camera::UseMMAP()
{
	req.memory = V4L2_MEMORY_MMAP;
}

bool Camera::SetPixFmt(unsigned format)
{
	struct v4l2_fmtdesc fmtdesc;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	for (fmtdesc.index = 0; ioctl(m_fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1; ++fmtdesc.index)
	{
		if (fmtdesc.pixelformat == format) {
			fmt.fmt.pix.pixelformat = format;
			return true;
		}
	}
	return false;
}

bool Camera::SetPixFmtMJPG()
{
	return SetPixFmt(V4L2_PIX_FMT_MJPEG);
}

bool Camera::SetPixFmtYUYV()
{
	return SetPixFmt(V4L2_PIX_FMT_YUYV);
}

void Camera::SetMemBufCount(unsigned n)
{
	req.count = n;
}

bool Camera::Init(unsigned w, unsigned h)
{
	check_device();

	fmt.fmt.pix.width = w;
	fmt.fmt.pix.height = h;

	init_device();
	cout << "camera init success" << endl;
	start_capturing();
	cout << "start_capturing success" << endl;

	return true;
}

unsigned Camera::GetImage(void* image)
{
	struct v4l2_buffer buf;
	DQBUF(buf);
	//assert(buf.index < req.count);
	memcpy(image, m_pBuff[buf.index].start, buf.bytesused);
	unsigned bytesused = buf.bytesused;
	QBUF(buf);
	return bytesused;
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
	cout << "Supported Format:" << endl;
	struct v4l2_fmtdesc fmtdesc;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	for (fmtdesc.index = 0; ioctl(m_fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1; ++fmtdesc.index)
	{
		const char* p = (const char*)&fmtdesc.pixelformat;
		printf("\t[%d: %c%c%c%c] %s\n", fmtdesc.index, p[0], p[1], p[2], p[3], reinterpret_cast<char*>(fmtdesc.description));
		enum_frame_sizes(m_fd, fmtdesc.pixelformat);
		printf("\n");
	}
}

int Camera::enum_frame_intervals(int dev, __u32 pixfmt, __u32 width, __u32 height)
{
	struct v4l2_frmivalenum fival;
	CLEAR(fival);
	fival.index = 0;
	fival.pixel_format = pixfmt;
	fival.width = width;
	fival.height = height;
	printf("\t  time interval between frames:");
	int ret;
	while ((ret = ioctl(dev, VIDIOC_ENUM_FRAMEINTERVALS, &fival)) == 0) {
		if (fival.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
			printf(" %u/%u", fival.discrete.numerator, fival.discrete.denominator);
		}
		else if (fival.type == V4L2_FRMIVAL_TYPE_CONTINUOUS) {
			printf(" {%u/%u ~ %u/%u}", fival.stepwise.min.numerator, fival.stepwise.min.numerator, fival.stepwise.max.denominator, fival.stepwise.max.denominator);
			break;
		}
		else if (fival.type == V4L2_FRMIVAL_TYPE_STEPWISE) {
			printf(" {%u/%u ~ %u/%u stepsize: %u/%u}", fival.stepwise.min.numerator, fival.stepwise.min.denominator, fival.stepwise.max.numerator, fival.stepwise.max.denominator, fival.stepwise.step.numerator, fival.stepwise.step.denominator);
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
	struct v4l2_frmsizeenum fsize;
	CLEAR(fsize);
	fsize.index = 0;
	fsize.pixel_format = pixfmt;

	int ret;
	while ((ret = ioctl(dev, VIDIOC_ENUM_FRAMESIZES, &fsize)) == 0) {
		if (fsize.type == V4L2_FRMSIZE_TYPE_CONTINUOUS) {
			printf("\tCONTINUOUS: {%u * %u} ~ {%u * %u}\n", fsize.stepwise.min_width, fsize.stepwise.min_height, fsize.stepwise.max_width, fsize.stepwise.max_height);
			break;
		}
		if (fsize.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
			printf("\tSTEPWISE: {%u * %u} ~ {%u * %u} stepsize: {%u : %u}\n", fsize.stepwise.min_width, fsize.stepwise.min_height, fsize.stepwise.max_width, fsize.stepwise.max_height, fsize.stepwise.step_width, fsize.stepwise.step_height);
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
	m_pBuff = nullptr;
	CLEAR(fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//V4L2_BUF_TYPE_PRIVATE;//
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	//V4L2_PIX_FMT_YUV420 : Planar formats with 1/2 horizontal and vertical chroma resolution, also known as YUV 4:2:0
	//V4L2_PIX_FMT_YUYV : Packed format with 1/2 horizontal chroma resolution, also known as YUV 4:2:2
	//V4L2_PIX_FMT_NV12 :
	//fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	CLEAR(req);
	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_USERPTR;

	struct stat st;
	if (-1 == stat(m_strDevName, &st))
		errno_exit("stat fail!!!");
	if (!S_ISCHR(st.st_mode))
		errno_exit("not char dev!!!");
	m_fd = open(m_strDevName, O_RDWR /*| O_NONBLOCK*/, 0);
	if (-1 == m_fd)
		errno_exit("open dev failed");
	cout << "open camera success" << endl;
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

	switch (req.memory)
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
		errno_exit("unsupported io method");
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


	if (-1 == xioctl(m_fd, VIDIOC_S_FMT, &fmt))
		errno_exit("VIDIOC_S_FMT fail");
	/* Note VIDIOC_S_FMT may change width and height. */
	if (-1 == xioctl(m_fd, VIDIOC_G_FMT, &fmt))
		errno_exit("VIDIOC_G_FMT fail");
	printf("{%d * %d} size: %d; bytesperline: %d\n", fmt.fmt.pix.width, fmt.fmt.pix.height, fmt.fmt.pix.sizeimage, fmt.fmt.pix.bytesperline);

	if (-1 == xioctl(m_fd, VIDIOC_REQBUFS, &req))
	{
		if (EINVAL == errno)
			errno_exit("VIDIOC_REQBUFS: dev does not support configured io method");
		else
			errno_exit("VIDIOC_REQBUFS");
	}
	//printf("req.capabilities: 0x%X, req.count: %d\n", req.capabilities, req.count);
	printf("req.count: %d\n", req.count);
	m_pBuff = new SBuff[req.count];
	if (req.memory == V4L2_MEMORY_MMAP)
		init_mmap();
	else
		init_user_prt();
	return true;
}

bool Camera::init_mmap(void) 
{
	for (unsigned i = 0; i < req.count; ++i)
	{
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if (-1 == xioctl(m_fd, VIDIOC_QUERYBUF, &buf))
			errno_exit("VIDIOC_QUERYBUF");
		printf("buf[%d] len: %d\n", i, buf.length);
		m_pBuff[i].length = buf.length;
		m_pBuff[i].start = mmap(nullptr, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, buf.m.offset);
		if (MAP_FAILED == m_pBuff[i].start)
			return false;
	}
	return true;
}

bool Camera::init_user_prt()
{
	for (unsigned i = 0; i < req.count; ++i)
	{
		m_pBuff[i].length = fmt.fmt.pix.sizeimage;
		m_pBuff[i].start = malloc(m_pBuff[i].length);
	}
	return true;
}

void Camera::uninit_device(void) 
{
	switch (req.memory)
	{
//	case IO_METHOD_READ:
//		//free(m_pBuff[0].start);
//		break;
	case V4L2_MEMORY_MMAP:
		for (unsigned i = 0; i < req.count; ++i)
			if (-1 == munmap(m_pBuff[i].start, m_pBuff[i].length))
				errno_exit("munmap");
		break;
	case V4L2_MEMORY_USERPTR:
		for (unsigned i = 0; i < req.count; ++i)
			free(m_pBuff[i].start);
		break;
	default:
		break;
	}
	delete[] m_pBuff;
}

bool Camera::start_capturing(void)
{
	for (unsigned i = 0; i < req.count; ++i)
	{
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = req.memory;
		buf.index = i;
		if (req.memory == V4L2_MEMORY_USERPTR)
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
	switch (req.memory)
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
	buf.memory = req.memory;
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


#include "datetime.h"
Camera::Frame::Frame(Camera* pCam)
	: m_pCam(pCam)
{
	CLEAR(m_buf);
	CTickHelper h("get frame");
	m_pCam->DQBUF(m_buf);
	start = m_pCam->m_pBuff[m_buf.index].start;
	len = m_buf.bytesused;
}

Camera::Frame::~Frame()
{
	m_pCam->QBUF(m_buf);
}
