CC := g++ -I ../../
CO := $(CC) -c -o

cam : main.cpp camera.o stream.o file.o str.o datetime.o args.o
	$(CC) $^ -o $@
camera.o : ../../io/camera.cpp
	$(CO) $@ $^
stream.o : ../../io/stream.cpp
	$(CO) $@ $^
file.o : ../../io/file.cpp
	$(CO) $@ $^
str.o : ../../str.cpp
	$(CO) $@ $^
datetime.o : ../../datetime.cpp
	$(CO) $@ $^
args.o : ../../utils/args.cpp
	$(CO) $@ $^
%.o : basic/%.cpp
	$(CO) $@ $^

	
.PHONY : clean clean-cam
clean :
	rm *.o
clean-cam :
	rm cam
