/* non-blocking soft-set example with timer
 */

#include "../motee.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>

volatile uint8_t id;

int main () {
    _delay_ms(100);
    moteeInit();

    //setup timer
    TIMSK = (1<<TOIE0);
    TCCR0 = (1<<CS02)|(1<<CS00);    //F_CPU/1024
    sei();

    //loop over all devices and check which was found
    //choose first found
    moteeSearch();
    int i;
    for (i = 0; i < 9; i ++) {
        if (motee_found[i]) {
            id = i;
            break;
        }
    }

    while (1) {

        if (!motee_during_soft[id]) {

            if (moteeGetSpeed(id) == 16) {
                moteeSoftSet(id, MOTEE_REVERSE, 16, 1000);
            } else {
                moteeSoftSet(id, MOTEE_FORWARD, 16, 1000);
            }

        }
    }
}

//timer is running with frequency F_CPU/1024, interrupt with
//frequency F_CPU/1024/256 -> 30Hz 
//so function is called each 33ms
SIGNAL(TIMER0_OVF_vect) {
    moteeSoftUpdate(33);
}
