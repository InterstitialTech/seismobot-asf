#include "max11200.h"
#include "delay.h"

void max_init(struct max_data *data) {

    struct spi_config config_spi;
    spi_get_config_defaults(&config_spi);

    config_spi.mux_setting = SPI_SIGNAL_MUX_SETTING_C;
    config_spi.pinmux_pad0 = PINMUX_PA14C_SERCOM0_PAD0;
    config_spi.pinmux_pad1 = PINMUX_PA15C_SERCOM0_PAD1;
    config_spi.pinmux_pad2 = PINMUX_PA04C_SERCOM0_PAD2;
    config_spi.pinmux_pad3 = PINMUX_UNUSED;

    enum status_code stat;
    stat = spi_init(&data->spi_instance, SERCOM0, &config_spi);
    if (stat != STATUS_OK) {while(1){}}

    spi_enable(&data->spi_instance);

    struct spi_slave_inst_config config_slave;
    spi_slave_inst_get_config_defaults(&config_slave);
    config_slave.ss_pin = PIN_PA05;
    spi_attach_slave(&data->slave, &config_slave);

    // config registers
    max_reg_write8(data, MAX_REG_CTRL1, 0b10100000);
    max_reg_write8(data, MAX_REG_CTRL2, 0b00001111);
    max_reg_write8(data, MAX_REG_CTRL3, 0b00011000);

    max_self_cal(data);

}

void max_self_cal(struct max_data *data) {

    uint8_t tx = 0b10010000;

    spi_select_slave(&data->spi_instance, &data->slave, true);
    spi_write_buffer_wait(&data->spi_instance, &tx, 1);
    spi_select_slave(&data->spi_instance, &data->slave, false);

    delay_blink();

}

uint8_t max_reg_read8(struct max_data *data, uint8_t addr) {

    uint8_t tx[2];
    uint8_t rx[2];

    addr = 0b1111 & addr;
    tx[0] = 0b11000001 | (addr << 1);
    tx[1] = 0;

    rx[0] = 0;
    rx[1] = 0;

    spi_select_slave(&data->spi_instance, &data->slave, true);
    spi_transceive_buffer_wait(&data->spi_instance, tx, rx, 2);
    spi_select_slave(&data->spi_instance, &data->slave, false);

    return rx[1];

}

void max_reg_write8(struct max_data *data, uint8_t addr, uint8_t val) {

    uint8_t tx[2];
    uint8_t rx[2];  // don't need?

    addr = 0b1111 & addr;
    tx[0] = 0b11000000 | (addr << 1);
    tx[1] = val;

    rx[0] = 0;
    rx[1] = 0;

    spi_select_slave(&data->spi_instance, &data->slave, true);
    spi_transceive_buffer_wait(&data->spi_instance, tx, rx, 2);
    spi_select_slave(&data->spi_instance, &data->slave, false);

}

uint32_t max_reg_read24(struct max_data *data, uint8_t addr) {

    uint8_t tx[4];
    uint8_t rx[4];

    addr = 0b1111 & addr;
    tx[0] = 0b11000001 | (addr << 1);
    tx[1] = 0;
    tx[2] = 0;
    tx[3] = 0;

    rx[0] = 0;
    rx[1] = 0;
    rx[2] = 0;
    rx[3] = 0;

    spi_select_slave(&data->spi_instance, &data->slave, true);
    spi_transceive_buffer_wait(&data->spi_instance, tx, rx, 4);
    spi_select_slave(&data->spi_instance, &data->slave, false);

    return (rx[0] << 3) | (rx[1] << 2) | (rx[2] << 1) | (rx[3] << 0);

}

