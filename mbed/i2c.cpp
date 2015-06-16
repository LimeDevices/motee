#include "i2c.h"

#ifndef I2C_SDA
#error "Please define I2C_SDA as sda pin for i2c communication, and I2C_SCL as scl pin"
#endif
#ifndef I2C_SCL
#error "Please define I2C_SDA as sda pin for i2c communication, and I2C_SCL as scl pin"
#endif

#define MOTEE_OK 0 //success
#define MOTEE_ERR_START 1 //can't start i2c
#define MOTEE_ERR_ADDR 2 //can't send slave address
#define MOTEE_ERR_SEND 3 //can't send byte
#define MOTEE_ERR_RESTART 4 //can't restart during receivin:
#define MOTEE_ERR_READ 10   

I2C i2c(I2C_SDA, I2C_SCL);

void i2cInit() {
    i2c.frequency(100000);
}

int8_t moteeSendByte(uint8_t address, uint8_t subaddress, uint8_t byte) {
    uint8_t data[2];
    data[0] = subaddress;
    data[1] = byte;

    i2c.start();

    if (!i2c.write((int)address, (const char*) data, 2))
        return MOTEE_ERR_SEND;

    i2c.stop();

    return MOTEE_OK;
}

int8_t moteeRecvByte(uint8_t address, uint8_t subaddress, uint8_t *byte) {
    i2c.start();

    if (!i2c.write((int)address, (const char*) &subaddress, 1))
        return MOTEE_ERR_SEND;

    if (!i2c.read((int)address, (char*) byte, 1))
        return MOTEE_ERR_READ;

    i2c.stop();

    return MOTEE_OK;
}
