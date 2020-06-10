#include "args.h"
#include "file.h"
#include "camera.h"

#define NUM_FRAME		10

int dev_id;
int print_info()
{
	Camera cam(dev_id);
	cam.PrintDevInfo();
	return 0;
}

int main(int argc, char ** argv) {
	int format_index;
	int W, H;
	CArgParser arg_parse(argc, argv, "A program to test UVC camera");
	auto flag_dev_id = arg_parse.Add(dev_id, "dev-id", 'd', true, "/dev/video<dev_id>");
	arg_parse.AddSub("info", print_info, "print supported format and image size").AddRef(flag_dev_id);
	arg_parse.Add(format_index, "format-index", 'f', true, "specify which format of image to snap");
	arg_parse.Add(W, "width", 'W', true, "specify image width");
	arg_parse.Add(H, "height", 'H', true, "specify image height");
	arg_parse.ParseArgs();
	Camera cam(dev_id);
	cam.SetMemBufCount(4);
	cam.Init(W, H, format_index);
	for (int i = 0; i < NUM_FRAME; ++i)
	{
		Camera::Frame frame = cam.GetFrame();
		CString strFileName;
		strFileName.Format("photo%03d.jpeg", i);
		CFile file(strFileName, "w");
		file.Write(frame.start, frame.len);
	}
	return 0;
}
