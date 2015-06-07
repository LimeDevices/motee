#ifndef _H_I2C
#define _H_I2C

#include <avr/io.h>
#include <inttypes.h>

//set i2c speed to 100kHz
#define F_I2C 100000L

//values that may be returned by i2c functions, same as in motee.h
#define MOTEE_OK            0   //success
#define MOTEE_ERR_START     1   //can't start i2c
#define MOTEE_ERR_ADDR      2   //can't send slave address
#define MOTEE_ERR_SEND      3   //can't send byte
#define MOTEE_ERR_RESTART   4   //can't restart during receivin:

void i2cInit();
int8_t i2cStart();
void i2cStop();

int8_t i2cSendAddress(uint8_t address);
int8_t i2cSendByte(uint8_t byte);
uint8_t i2cRecvByte();

int8_t moteeSendByte(uint8_t address, uint8_t subaddress, uint8_t byte);
int8_t moteeRecvByte(uint8_t address, uint8_t subaddress, uint8_t *byte);

#endif
