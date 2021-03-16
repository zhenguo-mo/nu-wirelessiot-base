#include "central.h"

void ble_evt_adv_report(ble_evt_t const* p_ble_evt) {

  // extract the fields we care about
  ble_gap_evt_adv_report_t const* adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
  uint8_t const* ble_addr = adv_report->peer_addr.addr; // array of 6 bytes of the address
  uint8_t* adv_buf = adv_report->data.p_data; // array of up to 31 bytes of advertisement payload data

  // Get mac address in big endian
  uint8_t mac_address[6] = {ble_addr[5], ble_addr[4], ble_addr[3], ble_addr[2], ble_addr[1], ble_addr[0]};
  
  // filter to only read the board's data
  if (mac_address[0] == 0xc0 && mac_address[1] == 0x98){
    printf("\n\nRECEIVED METRICS\n");
    printf("MAC Address: %x:%x:%x:%x:%x:%x\n", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);

    int8_t ping, rssi; 
    bool status;
    ping = adv_buf[14];
    rssi = adv_buf[15];
    status = adv_buf[16];

    printf("Connection Status: %s\n", status ? "CONNECTED" : "NOT CONNECTED");
    printf("RSSI: %i\n", rssi);
    printf("Ping: %ims\n", ping);
  }
}
 
void request_metrics(){
    uint8_t ble_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX] = {0x02, 0x01, 0x06, 0x02 , 0xFF, 0x01};

    simple_ble_adv_raw(ble_data, 6);
    printf("REQUESTING METRICS\n");
}

void connect_command() {
    uint8_t ble_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX] = {0x02, 0x01, 0x06, 0x02 , 0xFF, 0x00};

    simple_ble_adv_raw(ble_data, 6);
    printf("\nCONNECTING PERIPHERALS TO NETWORK\n");
}

void configure_button_requests(){
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
}

void begin_scanning(){
    scanning_start();
}