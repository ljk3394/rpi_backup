#include <stdio.h>
#include <wiringPi.h>
#include "Buzzer.h"


void buzzer_init(void){
    wiringPiSetupGpio();
    pinMode(buzzer_pin, PWM_OUTPUT);

    pwmSetClock(19);
    pwmSetMode(PWM_MODE_MS);
}

void beep(int note, int duration){
    pwmSetRange(1000000/note);
    pwmWrite(buzzer_pin, 1000000/note/2);
    delay(duration);
}

void welcomesound(void){
    beep(SOL_SHARP_L, 150);
    beep(RA_L, 150);
    beep(MI_L, 200);
    beep(NOTHING, 200);
    beep(RE_SHARP, 150);
    beep(FA_SHARP, 150);
    beep(DO_SHARP_H, 200);
    pwmWrite(buzzer_pin,0);
}


