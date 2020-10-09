srv : srv.cpp ../../io/stream.* ../../io/sock.* ../../datetime.* ../../str.* ../../thread/thread.h
	g++ $^ -I ../../ -l pthread -o $@ -g
cli : cli.cpp ../../io/stream.* ../../io/sock.*
	g++ $^ -I ../../ -o $@
