#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define SOCKPORT "8080" 

int main(int argc, char *argv[]) {
    int sockfd;
    struct addrinfo hints, *res;
    const char *message = argv[1];

    if (argc != 2) {
        fprintf(stderr,"usage: send <string>\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo("127.0.0.1", SOCKPORT, &hints, &res);
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(sockfd, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    ssize_t bytes_sent = send(sockfd, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("Send failed");
    } else {
        printf("Sent: %s\n", message);
    }

    close(sockfd);

    return 0;
}