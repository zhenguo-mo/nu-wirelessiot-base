#include "peripheral.h"
#include <stdlib.h>

// Pin configurations
#define UART_RX              NRF_GPIO_PIN_MAP(0, 8)
#define UART_TX              NRF_GPIO_PIN_MAP(0, 6)
#define UART_TX_BUF_SIZE     512
#define UART_RX_BUF_SIZE     512
#define MAX_TEST_DATA_BYTES     (15U)   

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

int main(void) {
  // init uart
  uart_init();

  simple_ble_app = simple_ble_init(&ble_config);
  begin_scanning();
  advertising_stop();
  while (1) {
    /* IF SCANNING SUCCESSFUL (global variable first <= latest global variable), DO RSSI and ADVERTISING */
    if (serviced_adv_id < latest_id){
      serviced_adv_id = latest_id;
      int rssi = esp_get_rssi();
      int ping = esp_get_ping();
      printf("RSSI: %i\n", rssi);
      printf("PING: %i\n", ping);
      fflush(stdout);

      begin_advertising(rssi, ping);
      nrf_delay_ms(20000);
      advertising_stop();
      printf("STOPPED BLE ADVERTISING\n");
    }
  }
}