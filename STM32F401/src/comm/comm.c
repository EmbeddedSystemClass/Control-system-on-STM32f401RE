#include "comm/comm.h"
#include "comm/comm_ModbusRtu.h"

TComm g_Comm;


void Comm_Init(TComm *p)
{
  InitDVibrChanelModbus(&g_Comm.mbDVibr);//���������� �������� ����������
  SerialCommInit(&g_Comm.mbDVibr);//InitModbus ��������� ������� � ���������
}



void Comm_Update(TComm *p)
{
  ModbusUpdate(&g_Comm.mbDVibr);
}