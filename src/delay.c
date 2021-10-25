#include "delay.h"

void delay_blink(void) {

    int i;

    for (i=0; i<100000; i++) {
        __asm__("nop");
    }

}
