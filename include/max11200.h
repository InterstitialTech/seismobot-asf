#ifndef MAX11200_H
#define MAX11200_H

#include "spi.h"

#define MAX_REG_STAT1  0x00
#define MAX_REG_CTRL1  0x01
#define MAX_REG_CTRL2  0x02
#define MAX_REG_CTRL3  0x03
#define MAX_REG_DATA   0x04
#define MAX_REG_SOC    0x05
#define MAX_REG_SGC    0x06
#define MAX_REG_SCOC   0x07
#define MAX_REG_SCGC   0x08

struct max_data {

    struct spi_module spi_instance;
    struct spi_slave_inst slave;

};

void max_init(struct max_data *data);

void max_self_cal(struct max_data *data);

uint8_t max_reg_read8(struct max_data *data, uint8_t addr);
void max_reg_write8(struct max_data *data, uint8_t addr, uint8_t val);
uint32_t max_reg_read24(struct max_data *data, uint8_t addr);

#endif
