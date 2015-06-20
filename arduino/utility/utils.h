#include <Arduino.h>

#ifndef BLOCKING_DELAY
#define BLOCKING_DELAY 10
#endif

void _delay() {
    delay(BLOCKING_DELAY);
}
