#include "peripheral.h"
#include <stdlib.h>

int main(void)
{
    uart_init();
    simple_ble_app = simple_ble_init(&ble_config);
    scanning_start();
    advertising_stop();
    nrf_delay_ms(5000);

    while (1)
    {
        printf("WAITING FOR REQUESTS\n");
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
        nrf_delay_ms(2000);

        if (service_request)
        {
            printf("\nGOT SERVICE REQUEST\n");

            bool status = esp_is_connected();
            int rssi = esp_get_rssi();
            int ping = esp_get_ping();

            printf("CONNECTION STATUS: %s\n", status ? "CONNECTED" : "NOT CONNECTED");
            printf("RSSI: %i\n", rssi);
            printf("Ping: %i\n", ping);
            fflush(stdout);

            printf("BROADCASTING METRICS\n");
            send_metrics(ping, rssi, status);
            nrf_delay_ms(5000);
            advertising_stop();
            printf("STOPPED BROADCASTING\n");
            service_request = false;
        }
        if (connection_request)
        {
            printf("\nGOT CONNECTION REQUEST\n");
            esp_init();
            connection_request = false;
            printf("CONNECTION ATTEMPTED\n");
        }
    }
}