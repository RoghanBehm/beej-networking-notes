#include <netdb.h>
#include <netinet/in.h>

//  ** Check out "Chapter 4: Jumping from IPv4 to IPv6" chapter for an overview of altering IPv4 network code to use IPv6 ** //
// You don't need to define these structs yourself, this is just whats inside of the structs

// Used to prep socket address structures for subsequent use
// also used in host name lookups and service name lookups

// Oftentimes, a call to getaddrinfo() will fill out struct for you, 
// so you only need to look in the sturct to retrieve the values
struct addrinfo {
int             ai_flags;       // AI_PASSIVE, AI_CANONNAME, etc.
int             ai_family;      // AF_INET, AF_INET6, AF_UNSPEC
int             ai_socktype;    // SOCK_STREAM, SOCK_DGRAM
int             ai_protocol;    // use 0 for "any"
size_t          ai_addrlen;     // size of ai_addr in bytes
struct sockaddr *ai_addr;       // struct sockaddr_in or _in6
char            *ai_canonname;  // full canonical hostname
struct addrinfo *ai_next;       // linked list, next node
};


// Holds socket address info for many ttypes of sockets
struct sockaddr {
    unsigned short  sa_family;      // address family, AF_xxx (AF_INET or AF_INET6 [IPV6])
    char            sa_data[14];    // 14 bytes of protocol address (dest. address and port number for socket)
};

// (IPv4 only--see struct sockaddr_in6 for IPv6)
// A pointer to sockaddr_in can be cast to a pointer to a sockaddr and vice versa
struct sockaddr_in {
short int           sin_family;     // Address family, AF_INET
unsigned short int  sin_port;       // Port number
struct in_addr      sin_addr;       // Internet address
unsigned char       sin_zero[8];    // Same size as struct sockaddr // Should be set to all zeros with memset() to pad struct to length of sockaddr
};

// (IPv4 only--see struct in6_addr for IPv6)
// Internet address (a structure for historical reasons)

//  if you have declared ina to be of type struct sockaddr_in, then ina.sin_addr.s_addr 
// references the 4- byte IP address (in Network Byte Order)
struct in_addr {
uint32_t s_addr; // that's a 32-bit int (4 bytes)
};


// (IPv6 only--see struct sockaddr_in and struct in_addr for IPv4)
struct sockaddr_in6 {
u_int16_t       sin6_family;    // address family, AF_INET6
u_int16_t       sin6_port;      // port number, Network Byte Order
u_int32_t       sin6_flowinfo;  // IPv6 flow information
struct in6_addr sin6_addr;      // IPv6 address
u_int32_t       sin6_scope_id;  // Scope ID
}; 

struct in6_addr {
unsigned char   s6_addr[16]; // IPv6 address
};

// Large enought hold both IPv4 and IPv6 structs for cases in which you don't know in advance
// if a call is going to fill out sockaddr with an IPv4 or v6 address
struct sockaddr_storage {
sa_family_t ss_family; // address family
// all this is padding, implementation specific, ignore it:
char __ss_pad1[_SS_PAD1SIZE];
int64_t __ss_align;
char __ss_pad2[_SS_PAD2SIZE];
};

// inet_pton converts and IP address into either a struct in_addr or struct in6_addr depending on whether you specify AF_INET or AF_INET6
// pton = "presentation to network", or perhaps "printable to network" - converts human-readble address to binary representation
struct sockaddr_in sa; // IPv4
struct sockaddr_in6 sa6; // IPv6
inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr)); // IPv4
inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr)); // IPv6


// inet_ntop is the inverse of pton - converts binary address to human-readable
// IPv4:

char ip4[INET_ADDRSTRLEN]; // space to hold the IPv4 string
struct sockaddr_in sa; // pretend this is loaded with something

inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);

printf("The IPv4 address is: %s\n", ip4);


// IPv6:

char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
struct sockaddr_in6 sa6; // pretend this is loaded with something

inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);

printf("The address is: %s\n", ip6);