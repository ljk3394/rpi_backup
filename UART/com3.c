#include "uartinterface.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "protocol.h"
#include "shmlib.h"
#include "Buzzer.h"

PortInfo portInfos[PORT_COUNT];

int main()
{
    int shmkey = 0x12345678;
    shm_t* shm = shm_initialize(shmkey);

    int data = 100;
    
        flaginfo.Key_Close_Flag=1;
    int uart_fds[PORT_COUNT] = {
        open_serial_port("/dev/ttyAMA0", B115200),
        open_serial_port("/dev/ttyAMA1", B115200),
        open_serial_port("/dev/ttyAMA2", B115200)
    };

    for (int i = 0; i < PORT_COUNT; ++i) {
        if (uart_fds[i] < 0) {
            fprintf(stderr, "Failed to open UART port %d\n", i);
            return 1;
        }
        memset(&portInfos[i], 0, sizeof(PortInfo));
    }

    // 데이터 읽기
    while (1) {
        for (int i = 0; i < PORT_COUNT; ++i) {
            process_recieved_data(uart_fds[i], &portInfos[i]);
        }
        if(flaginfo.Key_Close_Flag==1){
            update_data(shm,1);
            usleep(1000);
            //welcomelight뿌려주기
            printf("close\n");
            flaginfo.Key_Close_Flag=0;
        }
        if(flaginfo.Key_On_Flag==1){
            welcomesound();
            printf("key on\n");

            flaginfo.Key_On_Flag=0;
        }

    }


    for (int i = 0; i < PORT_COUNT; ++i) {
        close_serial_port(uart_fds[i]);
    }

    return 0;
}