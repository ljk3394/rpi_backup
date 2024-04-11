#include "protocol.h"
#include "uartinterface.h" // UART 인터페이스 헤더 파일 포함
#include <stdio.h>
#include <string.h>


 ProtocolFlag flaginfo;
 ProtocolData dataInfo;

void initialize_protocol(void) {
    memset(&flaginfo, 0, sizeof(flaginfo));
    memset(&dataInfo, 0, sizeof(dataInfo));
}

void Check_Protocol(const char* frame)
{
    if (strncmp(frame, "<WAH", 4) == 0 && frame[7] == '>') 
	{
        flaginfo.Air_Dir_Flag = 1;
        dataInfo.Air_Dir_Data = ((frame[4] - '0') * 100) + ((frame[5] - '0') * 10) + (frame[6] - '0');
    }
	else if (strncmp(frame, "<WAL", 4) == 0 && frame[7] == '>') 
	{
        flaginfo.Air_Speed_Flag = 1;
        dataInfo.Air_Speed_Data = ((frame[4] - '0') * 100) + ((frame[5] - '0') * 10) + (frame[6] - '0');
    }
	else if (strncmp(frame, "<WCH", 4) == 0 && frame[7] == '>') 
	{
        flaginfo.Seat_UpDown_Flag = 1;
        dataInfo.Seat_UpDown_Data = ((frame[4] - '0') * 100) + ((frame[5] - '0') * 10) + (frame[6] - '0');
    }
	else if (strncmp(frame, "<WCL", 4) == 0 && frame[7] == '>') 
	{
        flaginfo.Seat_RightLeft_Flag = 1;
        dataInfo.Seat_RightLeft_Data = ((frame[4] - '0') * 100) + ((frame[5] - '0') * 10) + (frame[6] - '0');
    } 
	else if (strncmp(frame, "<WSH", 4) == 0 && frame[7] == '>') 
	{
        flaginfo.Side_UpDown_Flag = 1;
        dataInfo.Side_UpDown_Data = ((frame[4] - '0') * 100) + ((frame[5] - '0') * 10) + (frame[6] - '0');
    }
	else if (strncmp(frame, "<WSL", 4) == 0 && frame[7] == '>') 
	{
        flaginfo.Side_RightLeft_Flag = 1;
        dataInfo.Side_RightLeft_Data = ((frame[4] - '0') * 100) + ((frame[5] - '0') * 10) + (frame[6] - '0');
    }
	else if (strncmp(frame, "<WK0001>", 8) == 0) 
	{
        flaginfo.Key_Close_Flag = 1;
    }
	else if (strncmp(frame, "<WK0000>", 8) == 0) 
	{
        flaginfo.Key_Far_Flag = 1;
    }
	else if (strncmp(frame, "<WK0010>", 8) == 0) 
	{
        flaginfo.Key_Off_Flag = 1;
    } 
	else if (strncmp(frame, "<WK0011>", 8) == 0) 
	{
        flaginfo.Key_On_Flag = 1;
    }
}

void process_recieved_data(int fd, PortInfo* portInfo) {
    char data;
    int bytes_read = read_from_serial_port(fd, &data, 1);

    if (bytes_read > 0) {
        portInfo->buffer[portInfo->buffer_index++] = data;

        if (data == '>') {
            char* frame_start = strchr(portInfo->buffer, '<');
            if (frame_start) {
                char* frame_end = strchr(frame_start, '>');
                if (frame_end) {
                    *frame_end = '\0'; // 프레임의 끝을 널 문자로 대체
                    Check_Protocol(frame_start);
                    memmove(portInfo->buffer, frame_end + 1, BUFFER_SIZE - (frame_end - portInfo->buffer) - 1);
                    portInfo->buffer_index -= (frame_end - frame_start) + 1;
                }
            }
        }

        if (portInfo->buffer_index >= BUFFER_SIZE - 1) {
            memset(portInfo->buffer, 0, BUFFER_SIZE);
            portInfo->buffer_index = 0;
        }
    }
}



void Comm_UartWrite(int uart_fd, char function1, char function2, uint32_t data) {
    char TX_Data[DATALENGTH + 1]; // +1 for null-termination
    snprintf(TX_Data, sizeof(TX_Data), "<%c%c%03d>", function1, function2, data);
    
    int bytes_written = send_to_serial_port(uart_fd, TX_Data, DATALENGTH);

    if (bytes_written > 0) {
        printf("Sent %d bytes: %s\n", bytes_written, TX_Data);
    } else {
        printf("Failed to send data.\n");
    }
}