CC := g++ -I ./basic/
CO := $(CC) -c -o

ser : main_ser.cpp basic/stream.* basic/sock.* basic/tick.* basic/thread.h
	g++ $^ -I ./basic/ -l pthread -o ser.out -g
cli : main_cli.cpp basic/stream.* basic/sock.*
	g++ $^ -I ./basic/ -o cli.out

cam : main_camera.cpp camera.o stream.o file.o str.o tick.o args.o
	$(CC) $^ -o $@
%.o : basic/%.cpp
	$(CO) $@ $^

	
.PHONY : clean clean-cam
clean :
	rm *.o
clean-cam :
	rm cam
