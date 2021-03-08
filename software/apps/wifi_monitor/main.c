#include "peripheral.h"
#include <stdlib.h>

// Pin configurations
#define UART_RX              NRF_GPIO_PIN_MAP(0, 8)
#define UART_TX              NRF_GPIO_PIN_MAP(0, 6)
#define UART_TX_BUF_SIZE     512
#define UART_RX_BUF_SIZE     512
#define MAX_TEST_DATA_BYTES     (15U)   

// error handler for UART
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


int main(void) {
  // init uart
  uart_init();
  int rssi = esp_get_rssi();
  int ping = esp_get_ping();
  // char hex_ping[5];
  // if (ping <= 0xFFFF){
  //   sprintf(hex_ping, "%04x", ping);
  // } else{
  //   sprintf(hex_ping, "FFFF");
  // }
  // hex_ping[4] = 0;
  // char val_one[3];
  // char val_two[3];
  
  // val_one[0] = hex_ping[0];
  // val_one[1] = hex_ping[1];
  // val_one[2] = 0;

  // val_two[0] = hex_ping[2];
  // val_two[1] = hex_ping[3];
  // val_two[2] = 0;

  // printf("FINISHED\n");
  printf("RSSI: %i\n", rssi);
  printf("PING: %i\n", ping);
  fflush(stdout);

  simple_ble_app = simple_ble_init(&ble_config);

  // Start Advertising
  uint8_t ble_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX] = {0x02, 0x01, 0x06, 0x08, 0x09, 0x54, 0x45, 0x43, 0x48, 0x34, 0x39, 0x37,
0x02, 0x0A, rssi, 0x03, 0xFF, ping};

  simple_ble_adv_raw(ble_data, 19);
  printf("Started BLE advertisements\n");

  while (1) {
    power_manage();
  }
}