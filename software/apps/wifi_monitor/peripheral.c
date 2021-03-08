#include "peripheral.h"
uint8_t serviced_adv_id = 0;
uint8_t latest_id = 0;

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

    simple_ble_adv_raw(ble_data, 18);
    printf("Started BLE advertisements\n");
}

// event handler for BLE
void ble_evt_adv_report(ble_evt_t const* p_ble_evt) {

    // extract the fields we care about
    ble_gap_evt_adv_report_t const* adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
    uint8_t const* ble_addr = adv_report->peer_addr.addr; // array of 6 bytes of the address
    uint8_t* adv_buf = adv_report->data.p_data; // array of up to 31 bytes of advertisement payload data
    // uint16_t adv_len = adv_report->data.len; // length of advertisement payload data


    if (ble_addr[5] == 0xc0 && ble_addr[4] == 0x98 && ble_addr[3] == 0xe5){
        latest_id = adv_buf[5];
    }
}

void begin_scanning(){
    scanning_start();
}

// event handler for UART
void uart_error_handle (app_uart_evt_t * p_event) {
  if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR) 
    {
    printf("communicatoin error \n");
    APP_ERROR_HANDLER(p_event->data.error_communication);
    } 
  else if (p_event->evt_type == APP_UART_FIFO_ERROR) 
    {
    printf("FIFO error \n");
    APP_ERROR_HANDLER(p_event->data.error_code);
    }
  else if (p_event->evt_type == APP_UART_DATA_READY) 
    {
      printf("data ready \n");
    }
  else if (p_event->evt_type == APP_UART_DATA) 
    {
      uint8_t ret;
      app_uart_get(&ret);
      char* ch = (char *) &ret;
      strcpy(&(buf.data[buf.size]), ch);
      printf("%c", buf.data[buf.size]);
      buf.size++;
    }
}


// initialization of UART
void uart_init(void) {
  uint32_t err_code;

  // This structure hold configuration information for the peripheral
  // configure RX and TX pins
  // no RTS or CTS pins with flow control disabled
  // no parity
  // baudrate 115200
  const app_uart_comm_params_t comm_params = {
    (uint32_t) 8, 
    (uint32_t) 6,
    0,
    0,
    APP_UART_FLOW_CONTROL_DISABLED,
    false,
    NRF_UARTE_BAUDRATE_115200
  };

  // actually initialize UART
  APP_UART_FIFO_INIT(
    &comm_params, 
    UART_RX_BUF_SIZE, 
    UART_TX_BUF_SIZE,
    uart_error_handle, 
    APP_IRQ_PRIORITY_LOW, 
    err_code);
  APP_ERROR_CHECK(err_code);

}