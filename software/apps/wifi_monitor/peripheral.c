#include "peripheral.h"
uint8_t serviced_adv_id = 0;
uint8_t latest_id = 1;

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

void begin_advertising(int ping, int rssi){
    // Start Advertising
    uint8_t ble_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX] = {0x02, 0x01, 0x06, 0x08, 0x09, 0x54, 0x45, 0x43, 0x48, 0x34, 0x39, 0x37,
    0x02, 0x0A, rssi, 0x03, 0xFF, ping};

    simple_ble_adv_raw(ble_data, 19);
    printf("Started BLE advertisements\n");
}

// event handler for BLE
void ble_evt_adv_report(ble_evt_t const* p_ble_evt) {

    // extract the fields we care about
    ble_gap_evt_adv_report_t const* adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
    uint8_t const* ble_addr = adv_report->peer_addr.addr; // array of 6 bytes of the address
    uint8_t* adv_buf = adv_report->data.p_data; // array of up to 31 bytes of advertisement payload data
    uint16_t adv_len = adv_report->data.len; // length of advertisement payload data


    if (ble_addr[5] == 0xc0 && ble_addr[4] == 0x98 && ble_addr[3] == 0xe5){
        latest_id = adv_buf[5];
    }
}

void begin_scanning(){
    scanning_start();
}