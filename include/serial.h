#ifndef SERIAL_H
#define SERIAL_H

#include <usart.h>

struct serial_data {

    struct usart_module usart_instance;

};

void serial_init(struct serial_data*);
void serial_hello(struct serial_data*);

#endif
