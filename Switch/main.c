
#include "swinterrupt3.h"
#include <stdio.h>
#include <wiringPi.h> 

int main() {
    setupSwitches();

    while (1) {
       delay(1000);
    }
    return 0;
}