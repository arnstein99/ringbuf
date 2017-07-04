CPPFLAGS += -g -Wall -pthread
LDFLAGS += -lstdc++ -lpthread
testring: testring.o
testring.o: posix_ringbuf.h ringbuf.h ringbuf.tcc
