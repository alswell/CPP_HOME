#include "args.h"
#include "file.h"
#include "camera.h"

#define NUM_FRAME		10

int main(int argc, char ** argv) {
	CArgParser arg_parse(argc, argv, "A program to test UVC camera");
	arg_parse.AddOption("dev_id", 'd', ARG_TYPE_INT, true, false, "/dev/video<dev_id>");
	arg_parse.AddOption("print_info", 'p', ARG_TYPE_BOOL, true, false, "print supported format and image size");
	arg_parse.AddOption("format_index", 'f', ARG_TYPE_INT, false, false, "specify which format of image to snap");
	arg_parse.AddOption("width", 'W', ARG_TYPE_INT, false, false, "specify image width");
	arg_parse.AddOption("height", 'H', ARG_TYPE_INT, false, false, "specify image height");
	arg_parse.ParseArgs();
	arg_parse.PrintResult();
	Camera cam((int)arg_parse["dev_id"]);
	if (arg_parse["print_info"])
	{
		cam.PrintDevInfo();
		exit(0);
	}
	if (arg_parse["width"] == NONE || arg_parse["height"] == NONE || arg_parse["format_index"] == NONE)
	{
		cout << "please specify --width, --height and --format_index" << endl;
		exit(-1);
	}
	cam.SetMemBufCount(4);
	cam.Init(arg_parse["width"], arg_parse["height"], arg_parse["format_index"]);
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

