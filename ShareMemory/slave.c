#include "shmlib.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    int shmkey = 0x12345678;
    shm_t* shm = shm_initialize(shmkey);

    int r, data;
    while (1) {
        printf("asdas\n");
        data = read_data(shm, &r);
        if (r != 3) { // r값을 검사하여 데이터 처리가 필요한 경우에만 출력
            printf("Slave read data: %d\n", data);
            shm->r = 3; //완료 플래그
        }
    }

    return 0;
}