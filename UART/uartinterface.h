//uartinterface.h

#ifndef UARTINTERFACE_H
#define UARTINTERFACE_H

#include <stddef.h>
#include <termios.h>

//uart serial port
#define     UART2   "/dev/ttyAMA3" //uart3
#define     UART3   "/dev/ttyAMA4" //uart4 
#define     UART4   "/dev/ttyAMA5" //uart5 
#define     UART5   "/dev/ttyAMA6" //uart6 

//baud rate
#define BAUDRATE B115200

//uart method
int open_serial_port(const char *device, int rate);
void close_serial_port(int fd);
int read_from_serial_port(int fd, char *buffer, size_t size);
int send_to_serial_port(int fd, const char *data, size_t length);

#endif