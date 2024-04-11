#include <stdio.h>
#include <string.h>
#include <wiringPi.h> // delay()
#include <wiringSerial.h>

#define BAUD 115200

int main(void) {
	int fd;
	char text[100];
	
	// 시리얼 오픈	
	if(fd = serialOpen("/dev/ttyAMA2", BAUD))
	{
		printf("ERROR");
		return 1;
	}

	printf("SERIAL TEST!\n");
	// 열린 시리얼 확인
	printf("%d", fd);

	// 반복으로 상대에게 받은 문자를 출력해준다..
	while(1)
	{
		if(serialDataAvail(fd))
		{
			printf("%c", serialGetchar(fd));
			serialFlush(fd);
		}
	}
	return 0;
}