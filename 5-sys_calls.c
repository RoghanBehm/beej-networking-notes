#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Utility function for resolving hostname to IP
//IPv4/6 agnostic - works for both
int getaddrinfo(const char *node,       // e.g. "www.example.com" or IP
                const char *service,    // e.g. "http" [or other service like gtp, telnet, smtp] or port number [e.g. "80" for HTTP]
                const struct addrinfo *hints,
                struct addrinfo **res);

// Sample call if you're a server who wants to listen on your host's IP address, 
// port 3490 - does not do any listening or network setup, just sets up structures            
int main () {
    int status;
    struct addrinfo hints; // points to a struct addrinfo which i would have already filled out (see chapter 4)
    struct addrinfo *servinfo; // will point to the results - basically the return result of getaddrinfo

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
    }

    // servinfo now points to a linked list of 1 or more struct addrinfos

    // ... do everything until you don't need servinfo anymore ....

    freeaddrinfo(servinfo); // free the linked-list
}

//Sample call if you're a client who wants to connect to a particular server (e.g., "www.example.net" port 3490)
//This example also does not connect, but sets up structures to be used later
void ba (void) {
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo; // will point to the results

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

    // get ready to connect
    status = getaddrinfo("www.example.net", "3490", &hints, &servinfo);

    // servinfo now points to a linked list of 1 or more struct addrinfos

    // etc. 
}

// Example socket() sys call usage
void ca (void) {
    int s;
    struct addrinfo hints, *res;

    // do the lookup
    // [pretend we already filled out the "hints" struct]
    getaddrinfo("www.example.com", "http", &hints, &res);

    // again, you should do error-checking on getaddrinfo(), and walk
    // the "res" linked list looking for valid entries instead of just
    // assuming the first one is good (like many of these examples do).
    // See the section on client/server for real examples.

    s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
}

// bind() sys call
#include <sys/types.h>
#include <sys/socket.h>
int bind(int sockfd, struct sockaddr *my_addr, int addrlen);

void da() {
    struct addrinfo hints, *res;
    int sockfd;

    // first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me - binds to the IP of the host. 
    //To bind a specific local IP, remove AI_PASSIVE and input an IP address for first arg in gettaddrinfo()

    getaddrinfo(NULL, "3490", &hints, &res);

    // make a socket:

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // bind it to the port we passed in to getaddrinfo():

    bind(sockfd, res->ai_addr, res->ai_addrlen);
}

// If "Address already in use" is encountered, this code will clear the socket
void addr_in_use() {
    int yes=1;
    //char yes='1'; // Solaris people use this

    // lose the pesky "Address already in use" error message
    if (setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
        perror("setsockopt");
        exit(1);
    }
}

// A note on bind: there are times when you do not need to call it.
// If connect()ing to a remote machine where you donn't care what your 
// local port is (e.g., telnet only cares about remote port), you can just
//  call connect() to check if socket is unbound, and will bind() to unused local port is necessary

// Making a socket connection to "www.example.com", port 3490:
void conne() {
    struct addrinfo hints, *res;
    int sockfd;

    // first, load up address structs wtih getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo("www.example.com", "3490", &hints, &res);

    // make a socket:

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // connect!

    connect(sockfd, res->ai_addr, res->ai_addrlen);
} // This code does not call bind because we don't carea bout our local port number, just where we're going (remote port)
  // The kernal will choose a local port ofr us, and the site we connect to will automatically get this information from us

// listen() listens for incoming connections on a port and IP address that is bound to a socket.
int listen(int sockfd, int backlog); //backlog is the number of connections allowed on the incoming queue - incoming connections wait in the queue until you accept() them


// Order of sys calls to listen for incoming connections:
void order() {
    getaddrinfo();
    socket();
    bind();
    listen();
    /* accept() goes here */
}
