#include <arpa/inet.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <sys/socket.h>
#include <unistd.h>

#define LISTEN_PORT 5000

volatile int server_sockfd;
volatile int client_sockfd;

void interact()
{
    char out[264];
    char in[256];

    FILE *fp = fopen("./addrs.txt", "w");
    fprintf(fp, "%p\n", &in);
    fclose(fp);

    strcpy(out, "Enter your name: ");

    send(client_sockfd, out, strlen(out), 0);
    recv(client_sockfd, in, 1024, 0);

    strcpy(out, "Hello: ");
    strcat(out, in);

    send(client_sockfd, out, strlen(out), 0);
}

/* Start server and redirect stdin and stdout to socket */
int setup_server()
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LISTEN_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* allow reuse of local address */
    const static int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    /* bind socket to local address */
    bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));
    listen(sockfd, 0);

    return sockfd;
}

void sighandler(int sig)
{
    close(client_sockfd);
    close(server_sockfd);
}

int main()
{
    /*signal(SIGINT, sighandler);*/
    server_sockfd = setup_server();

    struct sockaddr_in client_addr;
    unsigned sin_size = 0;

    while (true) {
        client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_addr, &sin_size); 
        interact(); 
        close(client_sockfd);     
    }

    return 0;
}
