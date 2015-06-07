#include <unistd.h>

void _sleep() {
    //100ms = 100000us
    usleep(100000);
}
