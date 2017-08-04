
#include "global/g_InterruptUpdate.h"
#include "peref/p_tempObserver.h"
#include "peref/p_voltControl.h"


#if defined(DEBUG)
void test_SPI (void);
void test_UART (void);

Byte test_data_Spi;
#endif



Uns count18kHz = 0,             //������� ���������� �������� 18 ���
    count2kHz = 0,              //������� ���������� �������� 2  ���
    count200Hz = 0,             //������� ���������� �������� 200 ��
    count50Hz = 0;              //������� ���������� �������� 10  ��



void InterruptUpdate(void)  //������� ��������� ����������
{
//����� ���� ������������
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  
//������� ���������� ������ �������
  count18kHz++;
  if (count18kHz == 9)
  {
    count18kHz = 0;
    count2kHz++;
    
    //tskCnt2kHz = 0;
    SerialCommTimings(&g_Comm.mbDVibr);
    VoltObserverUpdate();

    
    if (count2kHz == 2) //10
    {
      count2kHz = 0;
      count200Hz++;
      //tskCnt200Hz = 0;
     
    
      if (count200Hz == 4)
      {
        count200Hz=0;
        count50Hz++;
        //tskCnt50Hz = 0;
        
        if (count50Hz == 5)
        {
          count50Hz = 0;
          //tskCnt10Hz = 0
         
          TempObserverUpdate ();
          
#if defined(DEBUG)
    test_SPI ();
#endif
        }
      }
    }
  }
}




// �������� �������----------------------------------------------------
#if defined(DEBUG)

void test_SPI (void)
{
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, 0xAD);
}


void test_UART (void)
{
  GPIO_SetBits(GPIOA,GPIO_Pin_1);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);//���� ��������� ��������
  USART_SendData(USART2, (uint8_t) 'q');
  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}

#endif
//-----------------------------------------------------------------