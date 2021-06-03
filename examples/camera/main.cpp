#include "utils/args.h"
#include "io/file.h"
#include "io/camera.h"

int dev_id;
int print_info()
{
	Camera cam(dev_id);
	cam.PrintDevInfo();
	return 0;
}

int main(int argc, char ** argv) {
	bool mmap = false, yuv = false;
	int W, H, count = 10;
	CArgParser arg_parse(argc, argv, "A program to test UVC camera");
	auto flag_dev_id = arg_parse.Add(dev_id, "dev-id", 'd', true, "/dev/video<dev_id>");
	arg_parse.AddSub("info", print_info, "print supported format and image size").AddRef(flag_dev_id);
    arg_parse.Add(mmap, "mmap", 0, "use MMAP instead of UPTR as io-method");
    arg_parse.Add(yuv, "yuv", 0, "record image in format YUYV");
	arg_parse.Add(W, "width", 'W', true, "specify image width");
	arg_parse.Add(H, "height", 'H', true, "specify image height");
    arg_parse.Add(count, "count", 'c', false, "specify how many photos you want to take");
    arg_parse.ParseArgs();
	Camera cam(dev_id);
	cam.SetMemBufCount(1);
	if (mmap)
        cam.UseMMAP();
	const char* fmt = "photo%03d.jpeg";
	if (yuv) {
        cam.SetPixFmtYUYV();
        fmt = "photo%03d.yuv";
	}
	cam.Init(W, H);
	for (int i = 0; i < count; ++i)
	{
		Camera::Frame frame = &cam;
		CString strFileName;
		strFileName.Format(fmt, i);
		with_as(CFile(strFileName, "w"), file)
			file->Write(frame.start, frame.len);
	}
	return 0;
}
