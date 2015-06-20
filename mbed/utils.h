#ifndef _H_UTILS
#define _H_UTILS

#include "mbed.h"

#ifndef BLOCKING_DELAY
#define BLOCKING_DELAY 10
#endif

void _delay() {
    wait(BLOCKING_DELAY/1000.0);
}

#endif
