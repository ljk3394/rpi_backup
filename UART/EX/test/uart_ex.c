// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()


int main()
{
	struct termios newtio;
	int ttyfd;
 	char *ttyname = "/dev/ttyAMA2";
	ttyfd = open(ttyname, O_RDWR | O_NOCTTY);
	
	if(ttyfd < 0)
	{
		printf( ">> tty Open Fail [%s]\r\n ", ttyname);
		return -1;
	}
	memset( &newtio, 0, sizeof(newtio) );
	
	newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;

	//set input mode (non-canonical, no echo,.....)
	newtio.c_lflag     = 0;     // LF recive filter unused
	newtio.c_cc[VTIME] = 0;     // inter charater timer unused
	newtio.c_cc[VMIN]  = 0;     // blocking read until 1 character arrives

	tcflush( ttyfd, TCIFLUSH ); // inital serial port
	tcsetattr( ttyfd, TCSANOW, &newtio ); // setting serial communication
	printf( "## ttyo1 Opened [%s]\r\n", ttyname);
	

    if (ttyfd < 0) return 1;

    printf("Serial port opened and configured.\n");

    char read_buf [256];

    while (1) {
        memset(&read_buf, '\0', sizeof(read_buf));
        int num_bytes = read(ttyfd, &read_buf, sizeof(read_buf) - 1);

        if (num_bytes > 0) {
            printf("Read %i bytes. Received message: %s\n", num_bytes, read_buf);
        }
        // for(int i=0;i<100;i++)
        //     printf("%d",i);
        // Add a delay to ease the CPU usage
        usleep(100000); // 100 milliseconds
    }
    
	// 아래 부분은 테스트 코드 전용으로 실제로 사용하실때는 삭제바랍니다
	close(ttyfd); //close serial port
	return 0;
}