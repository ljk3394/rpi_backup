#ifndef SWINTERRUPT3_H
#define SWINTERRUPT3_H

#include <wiringPi.h>

// pinnumber
#define COLORSW 27
#define BRIGHTSW 28
#define MOTIONSW 29

//isr
void setupSwitches(void);
void colorisr(void);
void brightnessisr(void);
void motionisr(void);

#endif