#include "central.h"
#include <stdlib.h>


int main(void) {
  simple_ble_app = simple_ble_init(&ble_config);
  configure_button_renew_advertising();
  begin_scanning();

  while (1) {

    if (nrf_gpio_pin_read(BUTTON1)) {
        begin_advertising();
        nrf_delay_ms(5000);
    }
  }
}