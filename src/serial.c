#include "serial.h"

void serial_init(struct serial_data *data) {

    struct usart_config config_usart;
    usart_get_config_defaults(&config_usart);

    config_usart.baudrate    = 115200;
    config_usart.data_order = USART_DATAORDER_LSB;
    config_usart.transfer_mode = USART_TRANSFER_ASYNCHRONOUSLY;

    config_usart.pinmux_pad0 = PINMUX_UNUSED;
    config_usart.pinmux_pad1 = PINMUX_UNUSED;
    config_usart.pinmux_pad2 = PINMUX_PA24C_SERCOM1_PAD2;   // TX
    config_usart.pinmux_pad3 = PINMUX_PA25C_SERCOM1_PAD3;   // RX

    config_usart.mux_setting = USART_RX_3_TX_2_XCK_3;

    while (usart_init(&data->usart_instance, SERCOM1, &config_usart) != STATUS_OK) {}

    usart_enable(&data->usart_instance);

}

void serial_hello(struct serial_data *data) {

    uint8_t buffer[13] = "Hello world\r\n";

    usart_write_buffer_wait(&data->usart_instance, buffer, 13);

}

void get_dec_str (uint8_t* str, size_t len, uint32_t val) {

    uint8_t i;
    for(i=1; i<=len; i++) {
        str[len-i] = (uint8_t) ((val % 10UL) + '0');
        val/=10;
    }

    str[i-1] = '\0';

}

void serial_uint24(struct serial_data *data, uint32_t val) {

    // handles 24-bit uints, which can be 8 digits long (base 10)

    uint8_t buffer[10];
    get_dec_str(buffer, 8, val);
    buffer[8] = '\r';
    buffer[9] = '\n';

    usart_write_buffer_wait(&data->usart_instance, buffer, 10);

}
