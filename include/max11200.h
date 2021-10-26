#ifndef MAX11200_H
#define MAX11200_H

#include "spi.h"

struct max_data {

    struct spi_module spi_instance;
    struct spi_slave_inst slave;

};

void max_init(struct max_data *data);
uint32_t max_read(struct max_data *data);

#endif
