#include <samd09.h>
#include <system.h>
#include <port.h>

int main(void) {

    // system setup

    system_init(); // clock, board, events, extint, divas

    // configure LED pin
    struct port_config config_port_pin;
    port_get_config_defaults(&config_port_pin);
    config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
    port_pin_set_config(2, &config_port_pin);

    // super loop
    int i;
    while (1) {
        port_pin_set_output_level(PIN_PA02, true);
        for (i=0; i<100000; i++) {
            __asm__("nop");
        }
        port_pin_set_output_level(PIN_PA02, false);
        for (i=0; i<100000; i++) {
            __asm__("nop");
        }
    }

    return 0;

}
