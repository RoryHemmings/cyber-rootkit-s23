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

clean:
	rm *.o $(LIBNAME)
