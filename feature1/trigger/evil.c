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

