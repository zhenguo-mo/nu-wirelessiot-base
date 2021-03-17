#include "speedtest.h"
#include <time.h>

int main(){
    int socket_desc = create_socket();
    if (socket_desc == -1){
        printf("Could not create socket\n");
        return 1;
    }

    int connect_status = connect_socket(socket_desc);
    if (connect_status < 0){
        puts("connect error");
        return 1;
    }
    else{
        puts("Connected.");
    }
    const char* message = "GET /test.zip HTTP/1.1\r\nHost: mia.futurehosting.com\r\n\r\n";
    int send_status = socket_send(socket_desc, message);
    if (send_status < 0){
        puts("Send failed");
        return 1;
    } else{
        puts("Data sent\n");
    }

    char reply_buffer[9600];
    int total_len = 0;
    int start_time = (int)time(NULL);
    while (1){
        int received_len = recv(socket_desc, reply_buffer, sizeof(reply_buffer), 0);
        if (received_len <= 0){
            puts("recv finished\n");
            break;
        }
        total_len += received_len;
        if (total_len >= 12000000){
            puts("file is kind of long\n");
            break;
        }
    }
    int elapsed_time = (int)time(NULL) - start_time;
    printf("Time elapsed: %d\n", elapsed_time);
    printf("File downloaded size: %d\n", total_len);
    close_socket(socket_desc);

    return 0;
}