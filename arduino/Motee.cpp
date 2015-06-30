#include "Motee.hh"

#include "motee-c.h"
#include "i2c.h"

Motee::Motee(uint8_t id, bool reversed) {
    this->id = id;
    i2cInit();
    if (id <= 8)
        moteeReverse(id, reversed);
}

void Motee::begin() {
    moteeInit();
}

int8_t Motee::setSpeed(uint8_t direction, uint8_t speed) {
    return moteeSetSpeed(id, direction, speed);
}

int8_t Motee::setReverse(bool flag) {
    return moteeReverse(id, flag);
}

int8_t Motee::setStandby() {
    return moteeStandby(id);
}

int8_t Motee::setBrake() {
    return moteeBrake(id);
}

int8_t Motee::setSpeed(int8_t speed) {
    return moteeSetSpeedS(id, speed);
}

int8_t Motee::changeSpeed(int8_t change) {
    return moteeChangeSpeed(id, change);
}

int8_t Motee::softBlockingSet(uint8_t direction, uint8_t speed, uint32_t time) {
    return moteeSoftBlockingSet(id, direction, speed, time);
}

int8_t Motee::softSet(uint8_t direction, uint8_t speed, uint32_t time) {
    return moteeSoftSet(id, direction, speed, time);
}

int8_t Motee::softUpdate(uint32_t time) {
    return moteeSoftUpdate(time);
}
