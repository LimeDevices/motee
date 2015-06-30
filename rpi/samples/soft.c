#include <stdio.h>
#include "../motee.h"

int main () {
    moteeInit();

    int id = -1, i;

    moteeSearch();
    for (i = 0; i < 9; i ++) {
        if (motee_found[i]) {
            id = i;
            printf("found %d\n", i);
        }
    }

    if (id == -1) {
        printf("no motee found!\n");
        return 1;
    }

    printf("using %d\n", id);

    while(1) {
        moteeSoftBlockingSet(id, MOTEE_FORWARD, 32, 1000);
        moteeSoftBlockingSet(id, MOTEE_REVERSE, 32, 1000);
    }
}
