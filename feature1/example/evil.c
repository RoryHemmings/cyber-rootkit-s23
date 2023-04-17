#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>

#define TARGET "Hello World!"
#define REPLACEMENT "Goodbye World"

/*
 * The below example is essentially just a wrapper around the puts syscall that
 * filters the input under certain circumstances
 */

/* Hooked Syscall - overrides actual puts syscall */
int puts(const char *message)
{
	/* function pointer for original puts syscall (not yet initialized) */
	int (*new_puts)(const char *message);
	int result;

	/* 
	 * 1. Get address of next "puts" symbol from dynamic linker
	 * 2. assign new_puts to point to it
	 * 3. Now new_puts points to the REAL puts syscall
	 */
	new_puts = dlsym(RTLD_NEXT, "puts");

	/* If write is attempting to write our target string,
	 	 inject our subsequent payload instead */
	if (strcmp(message, TARGET) == 0)
		result = new_puts(REPLACEMENT);

 	/* Otherwise simply forward call to REAL puts */
	else
		result = new_puts(message);

	/* Return whatever was returned by the REAL puts syscall */
	return result;
}
