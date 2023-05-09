CC=gcc

all: target
target: target.c
	$(CC) -o target target.c -fno-stack-protector -z execstack -z norelro -O0 -no-pie

clean:
	rm target
