// poll(): Synchronous I/O Multiplexing
// sys call to let us know when data is ready and
// on which sockets, process sleeps meanwhile
#include <poll.h>
int poll(struct pollfd fds[], nfds_t nfds, int timeout); 
//nfds is the count of elements in array, timeout is a timeout in ms



// struct pollfd: array of info (which sockets to monitor for what)
struct pollfd {
    int fd; // the socket descriptor
    short events; // bitmap of events we're interested in
    short revents; // when poll() returns, bitmap of events that occurred
};
// events field is bitwise-OR of POLLIN (alert me when data is ready to
// recv() on this socket and POLLOUT (alert me when i can send() data to 
// this socket without blocking)

// So we have an array of pollfd structs, set an fd field for each element to
// a socket descriptor we're interested in monitoring. Then we set the events to indicate
// the type of events we're interested in




