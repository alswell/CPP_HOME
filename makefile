ser : main_ser.cpp basic/stream.* basic/sock.* basic/tick.* basic/thread.h
	g++ $^ -I ./basic/ -l pthread -o ser.out -g
cli : main_cli.cpp basic/stream.* basic/sock.*
	g++ $^ -I ./basic/ -o cli.out
cam : main_camera.cpp basic/camera.* basic/stream.* basic/file.* basic/str.* basic/tick.* basic/type.* basic/args.*
	g++ $^ -I ./basic/ -o camera.out
