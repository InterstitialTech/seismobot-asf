#include "max11200.h"

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

}

uint32_t max_read(struct max_data *data) {

    uint8_t tx[2];
    uint8_t rx[2];

    tx[0] = 0b11000010;
    tx[1] = 0;

    spi_select_slave(&data->spi_instance, &data->slave, true);
    spi_transceive_buffer_wait(&data->spi_instance, tx, rx, 2);
    spi_select_slave(&data->spi_instance, &data->slave, false);

    // 8 cycles, then 25 us break, then 8 more
    // mosi is correct for the first 8 cycles
    // miso is high as long as CS is low, resulting in rx=0xff

    return rx[1];

}
