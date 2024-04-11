#include <stdio.h>

#include <wiringPi.h>

 

#define buzzer_pin 18

 

int main(void) 

{

    wiringPiSetupGpio();

    pinMode(buzzer_pin, PWM_OUTPUT);

    

    pwmSetClock(19);

    pwmSetMode(PWM_MODE_MS);

    

    pwmSetRange(1000000/262);

    pwmWrite(buzzer_pin, 1000000/262/2);

    delay(3000);

    

    pwmWrite(buzzer_pin,0);

}