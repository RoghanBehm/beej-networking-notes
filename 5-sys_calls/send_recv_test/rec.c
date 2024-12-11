#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>

#define MYPORT "8080" 
#define BACKLOG 10

int main(void) {





    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, client_fd;
    char buffer[1024];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, MYPORT, &hints, &res);

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    listen(sockfd, BACKLOG);
    

  printf("Server listening on port %s...\n", MYPORT);


    while (1) {
        addr_size = sizeof their_addr;

        client_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
        if (client_fd == -1) {
            perror("accept");
            continue;
        }

        printf("Client connected!\n");

        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Received: %s\n", buffer);
        } else if (bytes_received == 0) {
            printf("Client disconnected.\n");
        } else {
            perror("recv");
        }


        close(client_fd);
    }
    close(sockfd);

    return 0;
}