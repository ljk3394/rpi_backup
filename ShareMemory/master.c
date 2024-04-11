#include "shmlib.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    int shmkey = 0x12345678;
    shm_t* shm = shm_initialize(shmkey);

    int data = 1000;
    while (1) {
        update_data(shm, data++);
        printf("fffff");
        printf("Master updated data to %d\n", data);
        // sleep(1);
    }

    return 0;
}