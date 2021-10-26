#include <samd09.h>
#include <system.h>
#include <port.h>

#include "delay.h"
#include "serial.h"

int main(void) {

    // system setup

    system_init(); // clock, board, events, extint, divas

    struct serial_data serial;
    serial_init(&serial);

    // configure LED pin
    struct port_config config_port_pin;
    port_get_config_defaults(&config_port_pin);
    config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
    port_pin_set_config(2, &config_port_pin);

    // super loop
    while (1) {

        port_pin_set_output_level(PIN_PA02, true);
        delay_blink();
        port_pin_set_output_level(PIN_PA02, false);
        delay_blink();

        serial_hello(&serial);

    }

    return 0;

}
