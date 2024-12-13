/*
2 ** client.c -- a stream socket client demo
3 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "9034" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    char *name = NULL; // Declare a pointer and initialize to NULL

    if (!name) {
        printf("Enter username: ");

        // Allocate memory to hold the input string
        name = malloc(256 * sizeof(char)); // Allocate enough space for 255 characters + null terminator
        if (!name) {
            perror("Memory allocation failed");
            return 1;
        }

        // Read input
        scanf("%255s", name); // Limit input to avoid buffer overflow
    }

    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    while (1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);

        // Monitor both socket and standard input
        FD_SET(sockfd, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        int max_fd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;

        // Wait for activity on either the socket or standard input
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }

        // Check if the server sent a message
        if (FD_ISSET(sockfd, &read_fds)) {
            char buf[MAXDATASIZE];
            int numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0);

            if (numbytes <= 0) {
                if (numbytes == 0) {
                    printf("Server closed the connection.\n");
                } else {
                    perror("recv");
                }
                break;
            }

            buf[numbytes] = '\0';
            printf("Server: %s\n", buf);
        }

        // Check if the user entered input
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char buf[MAXDATASIZE];
            char msg[MAXDATASIZE + 256]; // Buffer to hold username + message

            if (fgets(buf, sizeof buf, stdin) == NULL) {
                printf("Goodbye!\n");
                break;
            }

            buf[strcspn(buf, "\n")] = '\0'; // Strip newline character

            // Concatenate username and message
            snprintf(msg, sizeof msg, "%s: %s", name, buf);

            if (send(sockfd, msg, strlen(msg), 0) == -1) {
                perror("send");
                break;
            }
        }
}


    buf[numbytes] = '\0';

    printf("client: received '%s'\n", buf);

    close(sockfd);

    return 0;
}
