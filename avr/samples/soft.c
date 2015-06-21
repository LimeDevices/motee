/* blocking soft-set example
 */

#include "../motee-c.h"
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

uint8_t id;

int main () {
    _delay_ms(100);
    moteeInit();

    //loop over all devices and check which was found
    //choose first found
    int i;
    for (i = 0; i < 9; i ++) {
        if (motee_found[i]) {
            id = i;
            break;
        }
    }

    //in a loop chane speed from 16 to -16
    while(1) {
        moteeSoftBlockingSet(id, MOTEE_FORWARD, 16, 1000);
        moteeSoftBlockingSet(id, MOTEE_REVERSE, 16, 1000);
    }
}
