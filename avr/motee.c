#include "motee.h"

#include <avr/io.h>
#include <util/delay.h>

//complain if F_CPU isn't defined
#ifndef F_CPU
#error "F_CPU isn't defined for motee.c"
#endif

//default blocking delay to 10ms
#ifndef BLOCKING_DELAY
#define BLOCKING_DELAY 10
#endif

//global variables
bool motee_reversed[9], motee_found[9], motee_during_soft[9];
int8_t motee_speed[9], motee_state[9], motee_soft_target[9];
int32_t motee_soft_remainder[9], motee_soft_time[9];

//devices addresses, read address is higher by 1
const uint8_t motee_address[9] =
    {0xC0,0xC2,0xC4,0xC6,0xC8,0xCA,0xCC,0xCE,0xD0};

//registers' subaddresses
const uint8_t motee_reg_control = 0x00, motee_reg_fault = 0x01;

/****** I2C PRYMITIVES ******/
void i2cInit() {
    TWSR = 0;
    TWBR = (F_CPU/F_I2C - 16)/2;
}

int8_t i2cStart() {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
    if ((TWSR & 0xf8) != 0x08) 
        return MOTEE_ERR_START;
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
    TWCR = (1<<TWINT)|(1<<TWEN);
    while(!(TWCR&(1<<TWINT)));
    uint8_t ret = TWDR;

    return ret;
}

/****** I2C COMMUNICATION ******/
int8_t moteeSendByte(uint8_t address, uint8_t subaddress, uint8_t byte) {
    if (i2cStart() != MOTEE_OK) {
        i2cStop();
        return MOTEE_ERR_START;
    }

    if (i2cSendAddress(address) != MOTEE_OK) {
        i2cStop();
        return MOTEE_ERR_ADDR;
    }

    if (i2cSendByte(subaddress) != MOTEE_OK) {
        i2cStop();
        return MOTEE_ERR_SEND;
    }

    if (i2cSendByte(byte) != MOTEE_OK) {
        i2cStop();
        return MOTEE_ERR_SEND;
    }

    i2cStop();
    return MOTEE_OK;
}

int8_t moteeRecvByte(uint8_t address, uint8_t subaddress, uint8_t *byte) {
    if (i2cStart() != MOTEE_OK) {
        i2cStop();
        return MOTEE_ERR_START;
    }

    if (i2cSendAddress(address) != MOTEE_OK) {
        i2cStop();
        return MOTEE_ERR_ADDR;
    }

    if (i2cSendByte(subaddress) != MOTEE_OK) {
        i2cStop();
        return MOTEE_ERR_SEND;
    }

    i2cStart();
    i2cSendAddress(address|0x01);

    *byte = i2cRecvByte();

    i2cStop();
    return MOTEE_OK;
}

/****** USEFULL SMALL FUNCTIONS ******/

//adjust speed, so it will be in range <6; 63>
static uint8_t adjustSpeed(uint8_t speed) {
    if (speed == 0)
        return 0;
    if (speed < 6)
        return 6;
    if (speed > 63)
        return 63;
    return speed;
}

void _delay() {
    _delay_ms(BLOCKING_DELAY);
}

/****** MOTEE FUNCTIONS ******/

void moteeInit() {
    i2cInit();
    //default values: found (searching isn't mandatory), not reversed
    uint8_t i;
    for (i = 0; i < 9; i ++) {
        motee_reversed[i] = false;
        motee_found[i] = true;

        moteeStandby(i);
    }
}

int8_t moteeSearch() {
    uint8_t buff, found = 0;
    uint8_t i;
    for (i = 0; i < 9; i ++) {
        if (moteeReadFault(i, &buff) == MOTEE_OK) {
            found ++;
            motee_found[i] = true;
        } else {
            motee_found[i] = false;
        }
    }
    return found;
}

int8_t moteeSetSpeed(uint8_t id, uint8_t direction, uint8_t speed) {
    if (id > 8 || !motee_found[id])
        return MOTEE_ERR_WID;
    if (direction != MOTEE_FORWARD && direction != MOTEE_REVERSE)
        return MOTEE_ERR_DIR;

    speed = adjustSpeed(speed);

    //save signed speed
    if (direction == MOTEE_FORWARD)
        motee_speed[id] = speed;
    else
        motee_speed[id] = -1*speed;

    if (speed == 0)
        return moteeStandby(id);

    motee_state[id] = direction;

    //change to effective direction if reversed
    if (motee_reversed[id]) {
        if (direction == MOTEE_FORWARD)
            direction = MOTEE_REVERSE;
        else
            direction = MOTEE_FORWARD;
    }

    //disable soft speed change
    motee_during_soft[id] = false;

    //send byte, first two bits stands for direction, last six for speed
    return moteeSendByte(motee_address[id], 
            motee_reg_control, 
            (uint8_t)((speed << 2)|direction));
}

int8_t moteeReverse(uint8_t id, bool reverse) {
    if (id > 8 || !motee_found[id])
        return MOTEE_ERR_WID;
    motee_reversed[id] = reverse;
    return 0;
}

int8_t moteeStandby(uint8_t id) {
    if (id > 8 || !motee_found[id])
        return MOTEE_ERR_WID;

    //don't zero speed if it's in range<-6,6>, in this range
    //it can be only in result of moteeChangeSpeed, so don't
    //stop device in that state
    if (motee_speed[id] >= 6 || motee_speed[id] <= -6)
        motee_speed[id] = 0;
    motee_state[id] = MOTEE_STANDBY;

    //disable soft speed change
    motee_during_soft[id] = false;

    return moteeSendByte(motee_address[id], 
            motee_reg_control, 
            (uint8_t)(MOTEE_STANDBY));
}

int8_t moteeBrake(uint8_t id) {
    if (id > 8 || !motee_found[id])
        return MOTEE_ERR_WID;

    //as above
    if (motee_speed[id] >= 6 || motee_speed[id] <= -6)
        motee_speed[id] = 0;
    motee_state[id] = MOTEE_BRAKE;

    motee_during_soft[id] = false;

    return moteeSendByte(motee_address[id], 
            motee_reg_control, 
            (uint8_t)(MOTEE_BRAKE));
}

int8_t moteeReadFault(uint8_t id, uint8_t *byte) {
    if (id > 8 || !motee_found[id])
        return MOTEE_ERR_WID;
    return moteeRecvByte(motee_address[id], motee_reg_fault, byte);
}

int8_t moteeClearFault(uint8_t id) {
    if (id > 8 || !motee_found[id])
        return MOTEE_ERR_WID;
    return moteeSendByte(motee_address[id], motee_reg_fault, 0x80);
}

int8_t moteeSetSpeedS(uint8_t id, int8_t speed) {
    if (speed > 0) {
        return moteeSetSpeed(id, MOTEE_FORWARD, speed);
    } else {
        return moteeSetSpeed(id, MOTEE_REVERSE, -1*speed);
    }
}

int8_t moteeChangeSpeed(uint8_t id, int8_t change) {
    if (id > 8 || !motee_found[id])
        return MOTEE_ERR_WID;

    motee_speed[id] += change;

    if (motee_speed[id] > 63) {
        motee_speed[id] = 63;
    } else if (motee_speed[id] < -63) {
        motee_speed[id] = -63;
    }

    if (motee_speed[id] >= 6 || motee_speed[id] <= -6) {
        return moteeSetSpeedS(id, motee_speed[id]);
    } else {
        return moteeStandby(id);
    }
}

int8_t moteeSoftBlockingSet(uint8_t id, uint8_t direction, uint8_t speed, uint32_t time) {
    if (id > 8 || !motee_found[id])
        return MOTEE_ERR_WID;
    if (direction != MOTEE_FORWARD && direction != MOTEE_REVERSE)
        return MOTEE_ERR_DIR;
    
    speed = adjustSpeed(speed);

    int32_t steps=time/BLOCKING_DELAY;
    int8_t diff;

    if (direction == MOTEE_FORWARD)
        diff = speed - motee_speed[id];
    else
        diff = -1*(speed + motee_speed[id]);

    int8_t begin = motee_speed[id];

    int32_t i;
    for (i = 0; i < steps; i ++) {
        _delay();
         int8_t ret = moteeSetSpeedS(id, begin + i*diff/steps);
         if (ret != MOTEE_OK)
             return ret;
    }

    return moteeSetSpeed(id, direction, speed);
}

int8_t moteeSoftUpdate(uint32_t time) {
    int32_t newremainder;
    uint8_t i;
    for (i = 0; i < 9; i ++) {
        if (!motee_found[i] || !motee_during_soft[i])
            continue;

        if (motee_soft_time[i] > time) {
            newremainder = 
                (motee_soft_remainder[i] + (motee_soft_target[i]-motee_speed[i])*((int32_t)time))
                %motee_soft_time[i];

            moteeChangeSpeed(i, 
                    (motee_soft_remainder[i] + (motee_soft_target[i] - motee_speed[i])*((int32_t)time))
                    /motee_soft_time[i]);

            motee_during_soft[i] = true; //moteeChangeSpeed has set it to false

            motee_soft_remainder[i] = newremainder;
            motee_soft_time[i] -= time;
        } else {
            moteeSetSpeedS(i, motee_soft_target[i]);
            motee_soft_time[i] = 0;
            motee_during_soft[i] = false;
        }
    }
}

int8_t moteeSoftSet(uint8_t id, uint8_t direction, uint8_t speed, uint32_t time) {
    if (id > 8 || !motee_found[id])
        return MOTEE_ERR_WID;
    if (direction != MOTEE_FORWARD && direction != MOTEE_REVERSE)
        return MOTEE_ERR_DIR;
    
    speed = adjustSpeed(speed);
    motee_soft_time[id] = time;
    motee_soft_remainder[id] = 0;
    motee_during_soft[id] = true;

    if (direction == MOTEE_FORWARD)
        motee_soft_target[id] = speed;
    else
        motee_soft_target[id] = -1*speed;
}

int8_t moteeGetSpeed(uint8_t id) {
    if (id > 8 || !motee_found[id])
        return MOTEE_ERR_GETSPEED;
    return motee_speed[id];
}
