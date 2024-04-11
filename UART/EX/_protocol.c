// header : _protocol.c
// note : Collection of communication protocol related functions

#include "_protocol.h"

/* VARIABLES BEGIN */
ProtocolFlag FlagInfo;
ProtocolData DataInfo;
uint8_t RX_Data[DATALENGTH];
uint32_t Rx_flag = 0;
uint32_t Rx_it_source = 0;
uint8_t data = 0;
uint8_t index_rx = 0;
/* VARIABLES END */

void Uart1_Init()
{
   HAL_UART_Receive_IT(&huart1, (uint8_t*)RX_Data, DATALENGTH);
}

void Comm_UartWrite(uint8_t function1, uint8_t function2, uint32_t data)
{
   uint8_t TX_Data[8] = {0,};
   TX_Data[0] = '<';
   TX_Data[1] = 'R';
   TX_Data[2] = function1;
   TX_Data[3] = function2;
   TX_Data[4] = (data / 100) + '0';
   TX_Data[5] = ((data % 100) / 10) + '0';
   TX_Data[6] = (data % 10) + '0';
   TX_Data[7] = '>';
   HAL_UART_Transmit(&huart1,(uint8_t*)TX_Data, DATALENGTH, 10);
}

void Uart1_Handler()
{
	Rx_flag = __HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE);
	Rx_it_source = __HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE);

	if ((Rx_flag != RESET) && (Rx_it_source != RESET)) {
		 /* 1Byte copy */
		 data = (uint8_t)(huart1.Instance->RDR & (uint8_t)0x00FF);
		 RX_Data[index_rx] = data;
		 index_rx++;
	}
	else;

	if(index_rx >= 8)
	{
		Check_Protocol();

		index_rx = 0;
	}
}

void Check_Protocol()
{
	if(strncmp(RX_Data, "<WAH--->", 4) == 0 && RX_Data[7] == '>')
	{
		FlagInfo.Air_Dir_Flag = 1;
		DataInfo.Air_Dir_Data = ((RX_Data[4] -'0') * 100) + ((RX_Data[5] -'0') * 10) + (RX_Data[6] -'0');
	}
	else if(strncmp(RX_Data, "<WAL--->", 4) == 0 && RX_Data[7] == '>')
	{
		FlagInfo.Air_Speed_Flag = 1;
		DataInfo.Air_Speed_Data = ((RX_Data[4] -'0') * 100) + ((RX_Data[5] -'0') * 10) + (RX_Data[6] -'0');
	}
	else if(strncmp(RX_Data, "<WCH--->", 4) == 0 && RX_Data[7] == '>')
	{
		FlagInfo.Seat_UpDown_Flag = 1;
		DataInfo.Seat_UpDown_Data = ((RX_Data[4] -'0') * 100) + ((RX_Data[5] -'0') * 10) + (RX_Data[6] -'0');
	}
	else if(strncmp(RX_Data, "<WCL--->", 4) == 0 && RX_Data[7] == '>')
	{
		FlagInfo.Seat_RightLeft_Flag = 1;
		DataInfo.Seat_RightLeft_Data = ((RX_Data[4] -'0') * 100) + ((RX_Data[5] -'0') * 10) + (RX_Data[6] -'0');
	}
	else if(strncmp(RX_Data, "<WSH--->", 4) == 0 && RX_Data[7] == '>')
	{
		FlagInfo.Side_UpDown_Flag = 1;
		DataInfo.Side_UpDown_Data = ((RX_Data[4] -'0') * 100) + ((RX_Data[5] -'0') * 10) + (RX_Data[6] -'0');
	}
	else if(strncmp(RX_Data, "<WSL--->", 4) == 0 && RX_Data[7] == '>')
	{
		FlagInfo.Side_RightLeft_Flag = 1;
		DataInfo.Side_RightLeft_Data = ((RX_Data[4] -'0') * 100) + ((RX_Data[5] -'0') * 10) + (RX_Data[6] -'0');
	}
	else if(strncmp(RX_Data, "<WK0001>", 8) == 0)
	{
		FlagInfo.Key_Close_Flag = 1;
	}
	else if(strncmp(RX_Data, "<WK0000>", 8) == 0)
	{
		FlagInfo.Key_Far_Flag = 1;
	}
	else if(strncmp(RX_Data, "<WK0010>", 8) == 0)
	{
		FlagInfo.Key_Off_Flag = 1;
	}
	else if(strncmp(RX_Data, "<WK0011>", 8) == 0)
	{
		FlagInfo.Key_On_Flag = 1;
	}
	else;
}

void TestProtocol()
{
	Comm_UartWrite('A', 'H', DataInfo.Air_Dir_Data);
	Comm_UartWrite('A', 'L', DataInfo.Air_Speed_Data);
	Comm_UartWrite('C', 'H', DataInfo.Seat_UpDown_Data);
	Comm_UartWrite('C', 'L', DataInfo.Seat_RightLeft_Data);
	Comm_UartWrite('S', 'H', DataInfo.Side_UpDown_Data);
	Comm_UartWrite('S', 'L', DataInfo.Side_RightLeft_Data);

	if(FlagInfo.Key_Close_Flag == 1)
	{
		Comm_UartWrite('K', '0', 001);
		FlagInfo.Key_Close_Flag  = 0;
	}
	else if(FlagInfo.Key_Far_Flag == 1)
	{
		Comm_UartWrite('K', '0', 000);
		FlagInfo.Key_Far_Flag  = 0;
	}
	else if(FlagInfo.Key_Off_Flag == 1)
	{
		Comm_UartWrite('K', '0', 010);
		FlagInfo.Key_Off_Flag = 0;
	}
	else if(FlagInfo.Key_On_Flag == 1)
	{
		Comm_UartWrite('K', '0', 011);
		FlagInfo.Key_On_Flag  = 0;
	}
}