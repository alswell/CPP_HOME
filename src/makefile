libsys.a: stream.o file.o conf.o camera.o sync.o str.o sock.o uart.o tick.o type.o json.o dict.o fmt.o log.o
	ar -r $@ $^

.PHONY: clean
clean:
	rm *.o *.a
