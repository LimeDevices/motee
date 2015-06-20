#include <unistd.h>

#ifndef BLOCKING_DELAY 
#define BLOCKING_DELAY 10
#endif

void _sleep() {
    //10ms = 10000us
    usleep(BLOCKING_DELAY*1000);
}
