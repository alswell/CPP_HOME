ser : main_ser.cpp CppUtils/stream.* CppUtils/sock.* CppUtils/tick.* CppUtils/thread.h
	g++ $^ -I ./CppUtils/ -l pthread -o ser.out -g
cli : main_cli.cpp CppUtils/stream.* CppUtils/sock.*
	g++ $^ -I ./CppUtils/ -o cli.out
cam : main_camera.cpp CppUtils/camera.* CppUtils/str.* CppUtils/tick.*
	g++ $^ -I ./CppUtils/ -o camera.out
