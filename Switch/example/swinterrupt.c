#include <stdio.h>
#include <sys/time.h>
#include <wiringPi.h>

#define     COLORSW     27
#define     BRIGHTSW    28
#define     MOTIONSW    29

#define DEBOUNCE_TIME 100

static unsigned char color_state=0;
static unsigned char brightness_state=0;
static unsigned char motion_state=0;

long lastInterruptTime = 0; // 이전 인터럽트 시간 저장


long getmillis(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void handleSwitch(int pin) {
    long interruptTime = getmillis();

    if (interruptTime - lastInterruptTime > DEBOUNCE_TIME) {
        if (pin == COLORSW) {
            printf("COLORSW pressed\n");
            if(color_state>=4){
                color_state=0;
            }
            else{
                color_state++;
            }
            
        } else if (pin == BRIGHTSW) {
            printf("BRIGHTSW pressed\n");
            if(brightness_state>=4){
                brightness_state=0;
            }
            else{
                brightness_state++;
            }
        } else if (pin == MOTIONSW) {
            printf("MOTIONSW pressed\n");
            if(motion_state>=4){
                motion_state=0;
            }
            else{
                motion_state++;
            }
        }
    }
    lastInterruptTime = interruptTime;
}


void color(void){
    handleSwitch(COLORSW);
}

void brightness(void){
    handleSwitch(BRIGHTSW);
}

void motion(void){
    handleSwitch(MOTIONSW);
}


// void color(void){
//  struct timeval now;
//     gettimeofday(&now, NULL);
//     long int diff = (now.tv_sec - last_interrupt_time1.tv_sec) * 1000 + (now.tv_usec - last_interrupt_time1.tv_usec) / 1000;
    

//     if (diff > DEBOUNCE_TIME) { 
//         printf("color switch pressed");
//         color_state++;
//         if(color_state > 5) {
//             color_state = 0;
//         }
//     }
//     last_interrupt_time1 = now; 
// }

// void brightness(void){
//  struct timeval now;
//     gettimeofday(&now, NULL);
//     long int diff = (now.tv_sec - last_interrupt_time2.tv_sec) * 1000 + (now.tv_usec - last_interrupt_time2.tv_usec) / 1000;
    

//     if (diff > DEBOUNCE_TIME) { 
//         printf("brightness switch pressed\n");
//         brightness_state++;
//         if(brightness_state > 5) {
//             brightness_state = 0;
//         }
//     }
//     last_interrupt_time2 = now; 
// }

// void motion(void){
//  struct timeval now;
//     gettimeofday(&now, NULL);
//     long int diff = (now.tv_sec - last_interrupt_time3.tv_sec) * 1000 + (now.tv_usec - last_interrupt_time3.tv_usec) / 1000;
    

//     if (diff > DEBOUNCE_TIME) { 
//         printf("motion switch pressed\n");
//         motion_state++;
//         if(motion_state > 5) {
//             motion_state = 0;
//         }
//     }
//     last_interrupt_time3 = now; 
// }


int main(){
    wiringPiSetup();
    pinMode(COLORSW,INPUT);
    pinMode(BRIGHTSW,INPUT);
    pinMode(MOTIONSW,INPUT);

    pullUpDnControl(COLORSW, PUD_DOWN);        
    pullUpDnControl(BRIGHTSW, PUD_DOWN);
    pullUpDnControl(MOTIONSW, PUD_DOWN);

    if(wiringPiISR(COLORSW,INT_EDGE_FALLING,&color)<0){
        printf("isr1 setup error\n");
        return 1;
    }
     if(wiringPiISR(BRIGHTSW,INT_EDGE_FALLING,&brightness)<0){
        printf("isr2 setup error\n");
        return 1;
    }
     if(wiringPiISR(MOTIONSW,INT_EDGE_FALLING,&motion)<0){
        printf("isr3 setup error\n");
        return 1;
    }
 

    while(1){
        printf("color : %d, bright : %d, motion : %d\n",color_state,brightness_state,motion_state);
        delay(1000);
    }
}