#include "swinterrupt3.h"
#include <stdio.h>
#include <sys/time.h>

#define DEBOUNCE_TIME 100

unsigned char color_state = 0;
unsigned char brightness_state = 0;
unsigned char motion_state = 0;

static long lastInterruptTime = 0;

//return time in millis
static long getMillis(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

//interrupt handler
static void handleSwitch(int pin) {
    long interruptTime = getMillis();

    if (interruptTime - lastInterruptTime > DEBOUNCE_TIME) {
        if (pin == COLORSW) {
            color_state = (color_state >= 4) ? 0 : color_state + 1;
            printf("COLORSW pressed, color state: %d\n", color_state);
        } else if (pin == BRIGHTSW) {
            brightness_state = (brightness_state >= 4) ? 0 : brightness_state + 1;
            printf("BRIGHTSW pressed, brightness state: %d\n", brightness_state);
        } else if (pin == MOTIONSW) {
            motion_state = (motion_state >= 4) ? 0 : motion_state + 1;
            printf("MOTIONSW pressed, motion state: %d\n", motion_state);
        }
    }
    lastInterruptTime = interruptTime;
}

void colorisr(void) { handleSwitch(COLORSW); }
void brightnessisr(void) { handleSwitch(BRIGHTSW); }
void motionisr(void) { handleSwitch(MOTIONSW); }

void setupSwitches(void) {
    wiringPiSetup();
    pinMode(COLORSW, INPUT);
    pinMode(BRIGHTSW, INPUT);
    pinMode(MOTIONSW, INPUT);

    pullUpDnControl(COLORSW, PUD_DOWN);        
    pullUpDnControl(BRIGHTSW, PUD_DOWN);
    pullUpDnControl(MOTIONSW, PUD_DOWN);

    if (wiringPiISR(COLORSW, INT_EDGE_FALLING, &colorisr) < 0 ||
        wiringPiISR(BRIGHTSW, INT_EDGE_FALLING, &brightnessisr) < 0 ||
        wiringPiISR(MOTIONSW, INT_EDGE_FALLING, &motionisr) < 0) {
        printf("ISR setup error\n");
    }
}