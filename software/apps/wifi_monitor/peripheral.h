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
#include "simple_ble.h"
#include "nrf52840dk.h"

#define SSID "ATT6yzKXPi"
#define PWD "jjtn%g8n7zp4"

struct B
{
  char data[256]; 
  size_t size; 
} buf;

// BLUETOOTH ADVERTISING SETUP
// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:4e:xx:xx
        .platform_id       = 0x4E,   // used as 4th octect in device BLE address
        .device_id         = 0xAABB, // must be unique on each device you program!
        .adv_name          = "CS497", // used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

simple_ble_app_t* simple_ble_app;

/* BLE FUNCTIONS */

void begin_advertising();

/* END OF BLE FUNCTIONS */

// Function for connecting the ESP to the internet
void esp_init();

// Function for getting the ping from the network. Returns the ping.
int esp_get_ping();

// Function for getting the RSSI from the network. Returns the RSSI.
int esp_get_rssi();

// Function for running a speed test 
void esp_get_speed_test();

// Function to send commands to the ESP
void esp_send(const char* command); 

// Function which receives a message from the ESP and puts it 
// on the buffer 
void esp_receive(char* buf);

// Resets the global variable buffer (buf).
void reset_buffer();