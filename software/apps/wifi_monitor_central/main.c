#include "central.h"
#include <stdlib.h>


int main(void) {
  simple_ble_app = simple_ble_init(&ble_config);
  configure_button_requests();
  begin_scanning();
  advertising_stop();
  printf("\n\nREADY TO SEND REQUESTS\n");
  while (1) {
    if (!nrf_gpio_pin_read(BUTTON1)) {
        request_metrics();
        nrf_delay_ms(5000);
        advertising_stop();
        printf("STOPPED REQUESTING\n");
    }
    if (!nrf_gpio_pin_read(BUTTON2)) {
        connect_command();
        nrf_delay_ms(5000);
        advertising_stop();
        printf("STOPPED REQUESTING\n");
    }
    nrf_delay_ms(2000);
  }
}