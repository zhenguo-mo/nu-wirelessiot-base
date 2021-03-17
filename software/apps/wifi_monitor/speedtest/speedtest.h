#include <netinet/in.h>
#include<stdio.h> //printf
#include<string.h> //strcpy
#include<sys/socket.h>
#include<netdb.h>	//hostent
#include<arpa/inet.h>
#include<unistd.h>

#define HOSTNAME www.google.com

/* Creates a socket and returns the file descriptor */
int create_socket();

/* Connects socket to remote server */
int connect_socket(int socket_desc);

/* Sends request to download to socket */
int socket_send(int socket_desc, const char* message);

// /* Receives response from server */
// int socket_receive(int socket_desc);

/* Close socket */
void close_socket(int socket_desc);

/* Gets first ip of ips bound to hostname. Change to closest IP in future. RFU */
char* find_ip_by_hostname(char* hostname);