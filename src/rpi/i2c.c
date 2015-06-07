#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "i2c.h"

//TODO: check what should be included on rpi
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

//really not sure if it's ok, maybe put fds into an array?
void i2cInit() {
}

int8_t moteeSendByte(uint8_t address, uint8_t subaddress, uint8_t byte) {
    int fd;

    if ((fd = open(I2C_DEV, O_RDWR)) < 0)
        return MOTEE_ERR_START;

    if (ioctl (fd, I2C_SLAVE, address) < 0)
        return MOTEE_ERR_ADDR;

    if (i2c_smbus_write_byte_data(fd, subaddress, byte) < 0)
        return MOTEE_ERR_SEND;

    close(fd);
    return MOTEE_OK;
}

int8_t moteeRecvByte(uint8_t address, uint8_t subaddress, uint8_t *byte) {
    int fd;

    if ((fd = open(I2C_DEV, O_RDWR)) < 0)
        return MOTEE_ERR_START;

    if (ioctl (fd, I2C_SLAVE, address) < 0)
        return MOTEE_ERR_ADDR;

    *byte = i2c_smbus_read_byte_data(fd, subaddress);

    close(fd);
    return MOTEE_OK;
}
