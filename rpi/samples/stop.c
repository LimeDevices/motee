#include <stdio.h>
#include "../motee-c.h"

int main () {
    moteeInit();
    moteeSearch();

    int i;
    for (i = 0; i < 9; i ++) {
        if (motee_found[i]) {
            printf("stopping %d\n", i);
            moteeStandby(i);
        }
    }
}
