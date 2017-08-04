
#ifndef __MODBUS_EVENT_H
#define __MODBUS_EVENT_H

#include "comm_ModBusRtu.h"
#include "comm_ModbusFrame.h"
#include "comm_ModbusTimers.h"


//--Timer1_5----��� ������ ��� ����� ��������� ������� �� ����� 1.5------------------------------------------------------
__inline void BreakFrameEvent(TMbPort *hPort) 
{
 // USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
 // USART_ITConfig(USART2, USART_IT_TC, DISABLE);
  
  StopMbTimer(&hPort->Frame.TimerAsk);
}


//-Timer3_5----��� ������ ���� ������ 3.5 ������ ������ ������ ����� ������, ���������� ���������--
__inline void NewFrameEvent(TMbPort *hPort)
{
  hPort->Frame.NewMessage = true;
  hPort->Frame.WaitResponse = false;
  hPort->Frame.RxLength   = hPort->Frame.Data - hPort->Frame.Buf;//������ ��������� ���������
  hPort->Frame.Data       = hPort->Frame.Buf; // ����� ��������� ������ � ������
  
//  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//  USART_ITConfig(USART2, USART_IT_TC, DISABLE);
  
  StartMbTimer(&hPort->Frame.TimerPre);
}


//--�� ��������� ��������� ���������� �������� ������� (����� �� ����������)-------------------------------------------
__inline void PreambleEvent(TMbPort *hPort)
{
  hPort->Frame.Data = hPort->Frame.Buf;
  
 // USART_ITConfig(USART2, USART_IT_TC, ENABLE);
 // USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);

  GPIO_SetBits(GPIOA,GPIO_Pin_1);
  
  USART_SendData(USART2, (uint8_t) *hPort->Frame.Data++);
}


// ����� �������� ���������� ����� ������------------------------------------------
__inline void PostambleEvent(TMbPort *hPort)
{
  hPort->Frame.Data = hPort->Frame.Buf;  //����� ��������� �� ������ ������������� ������

 // USART_ITConfig(USART2, USART_IT_TC, DISABLE); // �������� �������
 // USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // ��������� ����������
  GPIO_ResetBits(GPIOA,GPIO_Pin_1);             // ����� ���� �������� (����������)
  
  hPort->Frame.WaitResponse = true;
  
  StartMbTimer(&hPort->Frame.TimerAsk);
}



//-------------------------------------------------------------------------------
__inline void NoAskEvent(TMbPort *hPort)
{    
 // USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
 // USART_ITConfig(USART2, USART_IT_TC, DISABLE);
     
  if (hPort->Stat.SlaveNoRespCount<65500)
    hPort->Stat.SlaveNoRespCount++;// ������� ���������
     
  if (hPort->Frame.RetryCounter<65500) 
    hPort->Frame.RetryCounter++;
    
  hPort->Frame.WaitResponse = false; //�� ����� ����� � ������� ����� �������� ������ ����������
  hPort->Stat.Status.bit.Error = 1;
  
  StartMbTimer(&hPort->Frame.TimerPre);
}





//------------------------------------------------------------------------
//-------------------------------------------------------------------------------
static void SendFrame(TMbPort *hPort)
{
  CrcPack(hPort);

//  USART_ITConfig(USART2, USART_IT_TC, ENABLE);
//  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
//  GPIO_SetBits(GPIOA,GPIO_Pin_1);
  hPort->Frame.WaitResponse = true;

  StartMbTimer(&hPort->Frame.TimerPre);
  hPort->Stat.TxMsgCount++;
}



/*
//-------------------------------------------------------------------------------
static void SendMasterResponse(TMbPort *hPort)
{
	hPort->Packet.Response    = hPort->Packet.Request;
	hPort->Packet.Request     = 0;
	hPort->Frame.RetryCounter = 0;
	hPort->Frame.WaitResponse = false;
}

*/

#endif