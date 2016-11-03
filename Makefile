CC = gcc
CXX = g++

INCLUDE_PATH = \
	-I. \
	-Iinclude \
	-I/usr/include \
	-I/usr/local/include

LIB_PATH = -L/lib -L/usr/lib -L/usr/local/lib
CFLAGS = $(INCLUDE_PATH) -DLINUX -std=gnu99
CPPFLAGS = 
LDFLAGS = $(LD_PATH) -lcjson -lm
EXECUTABLE = jconfig

$(EXECUTABLE):jconfig.o
	$(CC) -O2 -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o:%.c
	$(CC) -O2 -c -o $@ $< $(CFLAGS)

clean:
	rm -f  *.o $(EXECUTABLE)
