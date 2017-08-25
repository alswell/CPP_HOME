ser : main_ser.cpp CppUtils/*.*
	g++ $^ -I ./CppUtils/ -l pthread -o ser.out
cli : main_cli.cpp CppUtils/*.*
	g++ $^ -I ./CppUtils/ -o cli.out
