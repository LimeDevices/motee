#include "i2c.h"

//complain if F_CPU isn't defined
#ifndef F_CPU
#error "F_CPU isn't defined for i2c.c"
#endif

/****** I2C PRYMITIVES ******/
void i2cInit() {
    TWSR = 0;
    TWBR = (F_CPU/F_I2C - 16)/2;
}

int8_t i2cStart() {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
//     if ((TWSR & 0xf8) != 0x08) 
//         return MOTEE_ERR_START;
    return MOTEE_OK;
}

void i2cStop() {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

int8_t i2cSendAddress(uint8_t address) {
    TWDR = address;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xf8) != 0x18)
        return MOTEE_ERR_ADDR;
    return MOTEE_OK;
}

int8_t i2cSendByte(uint8_t byte) {
    TWDR = byte;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xf8) != 0x28)
        return MOTEE_ERR_SEND;
    return MOTEE_OK;
}

uint8_t i2cRecvByte() {
    TWCR = 0x84;
    while(!(TWCR & (1<<TWINT)));
    return TWDR;
}

/****** I2C COMMUNICATION ******/
int8_t moteeSendByte(uint8_t address, uint8_t subaddress, uint8_t byte) {
    if (i2cStart() != MOTEE_OK)
        return MOTEE_ERR_START;
    if (i2cSendAddress(address) != MOTEE_OK)
        return MOTEE_ERR_ADDR;
    if (i2cSendByte(subaddress) != MOTEE_OK)
        return MOTEE_ERR_SEND;
    if (i2cSendByte(byte) != MOTEE_OK)
        return MOTEE_ERR_SEND;
    i2cStop();
    return MOTEE_OK;
}

int8_t moteeRecvByte(uint8_t address, uint8_t subaddress, uint8_t *byte) {
    if (i2cStart () != MOTEE_OK)
        return MOTEE_ERR_START;
    if (i2cSendAddress(address) != MOTEE_OK)
        return MOTEE_ERR_ADDR;
    if (i2cSendByte(subaddress) != MOTEE_OK)
        return MOTEE_ERR_SEND;
    if (i2cStart() != MOTEE_OK)
        return MOTEE_ERR_RESTART;
    if (i2cSendAddress(address+0x01) != MOTEE_OK)
        return MOTEE_ERR_ADDR;
    *byte = i2cRecvByte();
    i2cStop();
    return MOTEE_OK;
}
