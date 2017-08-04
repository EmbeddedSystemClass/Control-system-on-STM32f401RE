#ifndef __CONFIG_H
#define __CONFIG_H

#if !defined(DEBUG)
#define DEBUG 0
#endif


#define UART_TYPE 0
#define SPI_TYPE 1


// ����������� ������������ ������
#include "global/g_hardware.h"	// ������������ ������������
#include "global/g_Rom.h"  //������������ ���������� � ������ � ����������
#include "comm/comm_ModbusRtu.h"
#include "stm32f4xx.h"
#include "global/std.h"

#endif