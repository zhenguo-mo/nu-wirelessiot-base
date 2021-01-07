// printf app
//
// Use RTT to print messages via printf

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "nrf52840dk.h"

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  nrf_gpio_cfg_output(LED1);

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized! Erroring in a second.\n");

  nrf_delay_ms(1000);
  volatile int invalid = *(int*)0x00000000;

  printf("You should never see this print %d\n", invalid);
  while (1);
}

