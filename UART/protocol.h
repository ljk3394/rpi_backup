#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define DATALENGTH 8
#define BUFFER_SIZE 256
#define PORT_COUNT 3

typedef struct {
    char buffer[BUFFER_SIZE];
    int buffer_index;
} PortInfo;

typedef struct{
	uint8_t Air_Dir_Flag;
	uint8_t Air_Speed_Flag;
	uint8_t Seat_UpDown_Flag;
	uint8_t Seat_RightLeft_Flag;
	uint8_t Side_UpDown_Flag;
	uint8_t Side_RightLeft_Flag;
	uint8_t Key_Close_Flag;
	uint8_t Key_Far_Flag;
	uint8_t Key_Off_Flag;
	uint8_t Key_On_Flag;
}ProtocolFlag;

typedef struct{
	uint8_t Air_Dir_Data;
	uint8_t Air_Speed_Data;
	uint8_t Seat_UpDown_Data;
	uint8_t Seat_RightLeft_Data;
	uint8_t Side_UpDown_Data;
	uint8_t Side_RightLeft_Data;
}ProtocolData;

extern ProtocolFlag flaginfo;
extern ProtocolData datainfo;

void initialize_protocol(void);
void process_recieved_data(int fd, PortInfo* portInfo);
void Check_Protocol(const char* frame);
void Comm_UartWrite(int uart_fd, char function1, char function2, uint32_t data);
#endif // PROTOCOL_H
