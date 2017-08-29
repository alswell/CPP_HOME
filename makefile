ser : main_ser.cpp CppUtils/ServSock.* CppUtils/Sock.* CppUtils/TickCount.* CppUtils/Thread.h
	g++ $^ -I ./CppUtils/ -l pthread -o ser.out
cli : main_cli.cpp CppUtils/Sock.*
	g++ $^ -I ./CppUtils/ -o cli.out
cam : main_camera.cpp CppUtils/Camera.* CppUtils/TickCount.*
	g++ $^ -I ./CppUtils/ -o camera.out
