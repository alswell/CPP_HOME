# C++ Utilities
## Examples:
### UVC camera
```
make cam
./camera.out -h
```
console print:
```
usage: ./camera.out [--help] --dev_id [--print_info] [--format_index] [--width] [--height] {}

description:
	A program to test UVC camera

positional arguments:
{} commands

optional arguments:
--help	-h  : show help message
--dev_id	-d <digit_value> : /dev/video<dev_id>
--print_info	-p  : print supported format and image size
--format_index	-f <digit_value> : specify which format of image to snap
--width	-W <digit_value> : specify image width
--height	-H <digit_value> : specify image height

```
### TCP server based on multi-thread
```
make ser
make cli
```
- start server first
```
# ./ser.out
socket ok!
bind ok!
listen ok!
Server start!
Accept!
thread ID: 139757315221248: hello; tick count: 4904981
client close!
Accept!
thread ID: 139757306828544: hello; tick count: 2620932
client close!
Accept!
^C
```
- then start client(send "hello")
```
# ./cli.out
# ./cli.out
```

