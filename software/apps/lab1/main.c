// Blink app
//
// Blinks an LED

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

// Pin configurations
#include "nrf52840dk.h"

// RTT stuff
#include "app_error.h"
#include "nrfx_gpiote.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

int main(void) {

  // Initialize.
  nrf_gpio_cfg_output(LED2);
  nrf_gpio_cfg_output(LED3);
  nrf_gpio_cfg_output(LED4);
  nrf_gpio_pin_set(LED2);
  nrf_gpio_pin_set(LED3);
  nrf_gpio_pin_set(LED4);
  nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);

  ret_code_t error_code = NRF_SUCCESS;
  nrf_gpio_cfg_output(LED1);

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // Enter main loop.
  int counter = 0;
  while (1) {
    // if (nrf_gpio_pin_read(BUTTON2)) {
    //   counter += 2;
    //   printf("Counter: %i\n", counter);
    // }

    // if (nrf_gpio_pin_read(BUTTON3)) {
    //   counter += 3;
    //   printf("Counter: %i\n", counter);
    // }

    // if (nrf_gpio_pin_read(BUTTON4)) {
    //   counter -= 4;
    //   printf("Counter: %i\n", counter);
    // }

    // if (counter < 0){
    //     nrf_gpio_pin_set(LED4);
    // }
    // else {
    //     nrf_gpio_pin_clear(LED4);
    // }

    // if (counter > 5){
    //     nrf_gpio_pin_set(LED1);
    // }
    // else {
    //     nrf_gpio_pin_clear(LED1);
    // }

    if (nrf_gpio_pin_read(BUTTON2)) {
      nrf_gpio_pin_set(LED2);
      
    } else {
      nrf_gpio_pin_clear(LED2);
      printf("Counter: %i\n", ++counter);
      nrf_delay_ms(500);
    }

    if (nrf_gpio_pin_read(BUTTON3)) {
      nrf_gpio_pin_set(LED3);
    } else {
      nrf_gpio_pin_clear(LED3);
      counter += 2;
      printf("Counter: %i\n", counter);
      nrf_delay_ms(500);
    }

    if (nrf_gpio_pin_read(BUTTON4)) {
      nrf_gpio_pin_set(LED4);
    } else {
      nrf_gpio_pin_clear(LED4);
      printf("Counter: %i\n", --counter);
      nrf_delay_ms(500);
    }

    if (counter >= 4){
        nrf_gpio_pin_clear(LED2);
    }
  }
}
