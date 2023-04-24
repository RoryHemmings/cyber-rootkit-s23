# Feature 2: Backdoor

This week you will use sockets develop the backdoor mechanism for our rootkit.

## Background

A backdoor is a covert mechanism we can use to bypass authentication and
control our target machine.

In order to communicate with our comprimised target machine, we need to
establish a network connection between our control host and the target machine.

After establishing connection, we can redirect stdin and stdout to the
connection socket, and execute the shell program on our target machine.

There are two models we can use to do this:

1. Bind shell - Target is treated as a server, and the attacker connects as
   a client.
2. Reverse Shell - Target becomes the client and connects to the attacker run
   server.

These methods have advantages and drawbacks, for example a rev shell is
cleaner, but requires hardcoding of CC server ip or proxy. These will be
further discussed in later weeks.

## Feature

Complete the function `start_bind()`.

When built, the executable `bind` should start a backdoor on
`localhost:1234`.

You should be able to to connect to this server using netcat, a simple
program that allows you to send/recieve text to/from a server.

`nc <target-ip> <port>` - establishes a connection with <target-ip> on port
<port>

## Hint 

The following syscalls may be helpful:

* dup2
* execve
