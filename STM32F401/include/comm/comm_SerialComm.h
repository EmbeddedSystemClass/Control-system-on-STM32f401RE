#ifndef __SERIAL_COMM_H
#define __SERIAL_COMM_H

#include "comm_ModbusRtu.h"


#define SPI_FRAME_BUF_SIZE	12	 // ������ ������� ������ �������

#define SPI_SLAVE_IN_CHECK      0x55     // ����������� ������, ������� �������� � DSP
#define SPI_SLAVE_OUT_CHECK     0xAA	 // ����������� ������, ������� ���������� �� DSP





typedef struct _TPpData {
  
  Uns X_VibrData;   // ������ ������������� � �������
  Uns Y_VibrData;   // ������ ������������� � �������
  Uns Z_VibrData;   // ������ ������������� � �������
  Uns Temp_P2;  // ����������� ���������� 2
  Uns Temp_T;   // ����������� �����������
  Uns Temp_ED1; // ����������� ����������������
  Uns Temp_P1;  // ����������� ���������� 1
  
  GPIO_StateUnion Discr_State_In;  // ������� �� ��� �� ���������� ������
  GPIO_AsuStateUnion Discr_ASU_In; // ��������� ��������� ���������� ������ (�������)
  GPIO_StateUnion Discr_State_Out;  // ������� �� ��� �� ���������� ������
  GPIO_AsuStateUnion Discr_ASU_Out; // ��������� ��������� ���������� ������ (�������)
  
  AdcChannelStateStructure AdcState; // ��������� ������� ���
  Byte DvibrErr; // ������ ������� �������� (������ ModBus)
  
  Byte *Data;                           // ��������� �� ������� ������
  Uns RxLength;       // ����� ��������� �����
  Uns TxLength;       // ����� ������������� �����
  Byte RxFrame[SPI_FRAME_BUF_SIZE];		// �����
  Byte TxFrame[SPI_FRAME_BUF_SIZE];		// �����
  
} TPpData, *TPpHandle;



void SerialCommInit(TMbHandle);
void ModbusReset(TMbHandle);
void InitDVibrChanelModbus(TMbHandle);

void SerialCommTimings(TMbHandle);

void ModbusUpdate(TMbHandle);

#endif