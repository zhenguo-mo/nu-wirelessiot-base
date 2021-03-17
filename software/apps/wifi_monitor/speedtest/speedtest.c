#include "speedtest.h"
#include <sys/socket.h>

/* Creates a socket and returns the file descriptor */
int create_socket(){
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    return socket_desc;
}

/* Connects socket to remote server */
int connect_socket(int socket_desc){
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("199.201.88.7");
	server.sin_family = AF_INET;
	server.sin_port = htons( 80 );
    return connect(socket_desc , (struct sockaddr *)&server , sizeof(server));
}

/* Sends request to download to socket */
int socket_send(int socket_desc, const char* message){
    return send(socket_desc, message, strlen(message), 0);
}

// /* Receives response from server */
// int socket_receive(int socket_desc);

/* Close socket */
void close_socket(int socket_desc){
    close(socket_desc);
}

/* Gets first ip of ips bound to hostname. Change to closest IP in future. RFU */
char* find_ip_by_hostname(char* hostname);