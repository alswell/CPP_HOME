CC := g++ -I ../../src
CO := $(CC) -c -o

cam : main.cpp camera.o stream.o file.o str.o datetime.o args.o
	$(CC) $^ -o $@
camera.o : ../../src/io/camera.cpp
	$(CO) $@ $^
stream.o : ../../src/io/stream.cpp
	$(CO) $@ $^
file.o : ../../src/io/file.cpp
	$(CO) $@ $^
str.o : ../../src/str.cpp
	$(CO) $@ $^
datetime.o : ../../src/datetime.cpp
	$(CO) $@ $^
args.o : ../../src/utils/args.cpp
	$(CO) $@ $^
%.o : basic/%.cpp
	$(CO) $@ $^

	
.PHONY : clean clean-cam
clean :
	rm *.o
clean-cam :
	rm cam
