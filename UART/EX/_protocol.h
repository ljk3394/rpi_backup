// header : protocol.h
// note : Collection of communication protocol related functions
#ifndef PROTOCOL_H
#define PROTOCOL_H

/* HEADER BEGIN */
#include "stm32f0xx_hal.h"
#include "string.h"
#include "stdlib.h"
/* HEADER END */

/* MACRO BEGIN */
#define DATALENGTH	8
/* MACRO END */

/* STRUCT & TYPE BEGIN */
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
/* STRUCT & TYPE END */

/* VARIABLES BEGIN */
extern uint8_t RX_Data[DATALENGTH];
extern UART_HandleTypeDef huart1;
extern ProtocolFlag FlagInfo;
extern ProtocolData DataInfo;
extern uint32_t Rx_flag;
extern uint32_t Rx_it_source;
extern uint8_t data;
extern uint8_t index_rx;
/* VARIABLES END */

/* FUNCTION BEGIN */
void Uart1_Init();
void Uart1_Handler();
void Check_Protocol();
void Comm_UartWrite(uint8_t function1, uint8_t function2, uint32_t data);
void TestProtocol();
/* FUNCTION END */

#endif