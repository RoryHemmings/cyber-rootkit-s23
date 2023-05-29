CC=gcc

all: setup

setup:
	docker compose up -d

shell:
	docker exec -it remote_target-mw-target-1 bash

target: target.c
	$(CC) -o target target.c -fno-stack-protector -z execstack -z norelro -O0 -no-pie

clean:
	docker compose down -v --rmi all --remove-orphans
	rm target
