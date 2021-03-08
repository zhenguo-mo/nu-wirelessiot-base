#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "app_uart.h"
#include "nrf_uarte.h"
#include "nrf_power.h"

// Must be able to request individual metrics
// Configure buttion IO to get metrics
// Advertise Bluetooth
// Peripherals listening for Central
// When it hears command, run tests
// Put everything in manufacture data

void configure_buttons();
void request_metric();