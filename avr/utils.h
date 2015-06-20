#include <util/delay.h>

#ifndef BLOCKING_DELAY
#define BLOCKING_DELAY 10
#endif

void _delay() {
    _delay_ms(BLOCKING_DELAY);
}
