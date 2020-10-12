srv : srv.cpp ../../src/io/stream.* ../../src/io/sock.* ../../src/datetime.* ../../src/str.* ../../src/thread/thread.h
	g++ $^ -I ../../src -l pthread -o $@ -g
cli : cli.cpp ../../src/io/stream.* ../../src/io/sock.*
	g++ $^ -I ../../src -o $@
