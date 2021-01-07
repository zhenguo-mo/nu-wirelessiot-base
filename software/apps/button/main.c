// Buttons app
//
// Use buttons and switches to control LEDs

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

// Pin definitions
#include "nrf52840dk.h"

int main(void) {

  // Initialize.
  nrf_gpio_cfg_output(LED1);
  nrf_gpio_cfg_output(LED2);
  nrf_gpio_cfg_output(LED3);
  nrf_gpio_cfg_output(LED4);
  nrf_gpio_pin_set(LED1);
  nrf_gpio_pin_set(LED2);
  nrf_gpio_pin_set(LED3);
  nrf_gpio_pin_set(LED4);
  nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);

  // Enter main loop.
  while (1) {
    if (nrf_gpio_pin_read(BUTTON1)) {
      nrf_gpio_pin_set(LED1);
    } else {
      nrf_gpio_pin_clear(LED1);
    }

    if (nrf_gpio_pin_read(BUTTON2)) {
      nrf_gpio_pin_set(LED2);
    } else {
      nrf_gpio_pin_clear(LED2);
    }

    if (nrf_gpio_pin_read(BUTTON3)) {
      nrf_gpio_pin_set(LED3);
    } else {
      nrf_gpio_pin_clear(LED3);
    }

    if (nrf_gpio_pin_read(BUTTON4)) {
      nrf_gpio_pin_set(LED4);
    } else {
      nrf_gpio_pin_clear(LED4);
    }
  }
}

