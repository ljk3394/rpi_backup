#include <stdio.h>
#include <wiringPi.h>

#define pin 29

int main(){
    wiringPiSetup();
    pinMode(21,INPUT);
   	while(1)
	{
		if(digitalRead(pin)==HIGH){
			printf("Button was pressed !! \n");
			
		} else {
			printf("Button was NOT pressed !! \n");
			
		}
		delay(1000);
	}
}