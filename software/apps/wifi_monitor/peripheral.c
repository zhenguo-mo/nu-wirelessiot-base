#include "peripheral.h"

void esp_send(const char* command) {
    uint8_t len = strlen(command); 
    uint8_t* data = (uint8_t *) command;
    uint32_t i = 0;
    while (i < len) { 
        // printf("sending %c \n", data[i]);
        while (app_uart_put(data[i]) != NRF_SUCCESS);
        
        i++;
    } 
}

// waits for a command to finish and prepares for the next one
int esp_wait() { 
    while (strstr((const char *) &buf.data, "OK\r\n") == NULL) {
    printf("waiting 1 second\n");
    printf("buffer: %s\n", buf.data);
    nrf_delay_ms(1000); // wait until you read "OK" in the response
    }
    // reset the buffer
    printf("resetting buffer");
    printf("old buffer %s", buf.data);
    buf.data[0] = 0;
    buf.size = 0;
    printf("new buffer %s", buf.data);
    return 0;
}

void esp_init() { 
    char connect[128];
    esp_send("AT+CWMODE=1\r\n"); // set ESP to client mode
    nrf_delay_ms(3000);

    sprintf(connect, "AT+CWJAP=\"%s\",\"%s\"\r\n",SSID, PWD); // connect to network
    printf("connect is: %s \n", connect);
    esp_send(connect);
    nrf_delay_ms(10000);
}

void reset_buffer() {
    printf("RESET_BUFFER\n");
    buf.size = 0;
    memset(buf.data, 0, sizeof(buf.data));
}

int esp_get_ping() { 
    int ret;
    esp_send("AT+PING=\"www.google.com\"\r\n");
    nrf_delay_ms(2000);

    sscanf(&(buf.data[buf.size-10]), "%d", &ret); // extract the ping
    reset_buffer();
    return ret;
}

int esp_get_rssi() { 
    int ret;
    esp_send("AT+CWJAP?\r\n");
    nrf_delay_ms(1000);
    sscanf(&(buf.data[buf.size-10]), "%d", &ret); // extract the rssi
    reset_buffer();
    return ret * (-1);
}

/* BLE FUNCTIONS */

void begin_advertising(){
    simple_ble_app = simple_ble_init(&ble_config);

    // Start Advertising
    simple_ble_adv_only_name();
    printf("Started BLE advertisements with name %s\n", ble_config.adv_name);
    
}