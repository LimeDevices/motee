#ifndef _H_I2C
#define _H_I2C

#include <inttypes.h>

//values that can be returned
#define MOTEE_OK            0   //success
#define MOTEE_ERR_START     1   //can't start i2c
#define MOTEE_ERR_ADDR      2   //can't send slave address
#define MOTEE_ERR_SEND      3   //can't send byte
#define MOTEE_ERR_RESTART   4   //can't restart during receivin:

//target i2c device
#ifndef I2C_DEV
#define I2C_DEV "/dev/i2c-0"
#endif

void i2cInit();
int8_t moteeSendByte(uint8_t address, uint8_t subaddress, uint8_t byte);
int8_t moteeRecvByte(uint8_t address, uint8_t subaddress, uint8_t *byte);

#endif
