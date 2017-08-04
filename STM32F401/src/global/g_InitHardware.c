/*-----------------------------------
InitHardware

������ ������������ ������������
-------------------------------------*/
#include "global/g_hardware.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void InitClock(void);
void InitTimer(void);

void InitGpio(void);
void InitSpi(void);
void InitUart(void);
void InitAdc(void);

void InitDma (void);    //��� �������� � ADC ������ ��� ������ �������
                        //� 1 ������� � ���������� ���� �����
                        //(����� injected channel, �� �� ����� 4, � ������� 6)

__IO uint16_t ADC1ConvertedValue = 0;

void InitHardware(void)
{
  InitClock();
  InitTimer();
  
  InitGpio();
  InitUart();
  InitDma();
  InitAdc();
  
  InitSpi();
}



//��������� ��� ������������ (�� �� �����, ���� �� ������������ �������)
void InitClock(void)
{
  RCC_PLLCmd(DISABLE);  //��������� PLL, �� ���������� ������ �����������
  
  do
  {
    RCC_HSEConfig(RCC_HSE_ON); //������� ������� ��������
  }
  while (!RCC_WaitForHSEStartUp());     //�������� ���������
    
  RCC_PLLConfig(RCC_PLLSource_HSE,      //������� ���������
                8,                      //�������� M PLL (��� ��� �������� �� ����� PLL)
                84,                     //��������� N PLL  = 16*84/(8*2)=84 ���
                2,                      //�������� P ��� ������� SYSCLK (2,4,6,8)
                4);                     //(�� ������������) �������� Q ��� USB, OTG FS, SDIO, RNG clocks (4-15)
  
  do
  {
    RCC_PLLCmd(ENABLE);
  }
  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)!=SET);

//  RCC_HSICmd(DISABLE);  //�������� ���������� ���������

  RCC_HCLKConfig(RCC_SYSCLK_Div1);//�������� ����� ������
  RCC_PCLK1Config(RCC_HCLK_Div2);//�������� ����� ������������ ����� 1 (���������, ����� 42��� (������������))
  RCC_PCLK2Config(RCC_HCLK_Div1);//�������� ����� ������������ ����� 2 (������� 84 ��� �����������)
  
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);       // ���� �� ���������� 32.768 ���
  RCC_RTCCLKCmd(ENABLE);                        // ��������� ����� RTC

  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // SW - ����� ������������ � PLL (����� HSI, HSE)
  RCC_HSICmd(DISABLE);
  SystemCoreClockUpdate();
 
}



//��������� ������� 2 (� ���� )
void InitTimer(void)
{
 // NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
  
  //������ � ���� APB2 timer clock = 84 MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
  
  TIM_BaseInitStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock/1000000)-1; //�������� �� ������� 1���
  TIM_BaseInitStructure.TIM_Period = 55;      // =1���/18��� = 55
  TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // �� ���� �� TIM_Period
  TIM_TimeBaseInit(TIM2, &TIM_BaseInitStructure);
  
//���������� ������� 2
/*  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  */

//��������� ����������
  NVIC_EnableIRQ(TIM2_IRQn);
//���������� ����������
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//������ �������
  TIM_Cmd(TIM2, ENABLE);
}




void InitGpio(void)
{
 
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
  
// Enable the GPIOA peripheral
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  
//Input_Output Ports
//PA-----------------------------------------------------------------------------
//PA0 - STOP_24 - �� ��� �� �������� ������� 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;     //����� ��� � �����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  //����� - ����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // ��������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���� �������(1,0) (��� ���� � ������� �������)
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // ����������� �������� ������
  GPIO_Init(GPIOA, &GPIO_InitStructure);        //������   
//  GPIO_WriteBit(GPIOA,GPIO_Pin_0,0);            
  GPIO_ResetBits(GPIOA, GPIO_Pin_0);      //��������� ��������
  
//PA1 - UART RTS
//PA2 - UART Tx
//PA3 - UART Rx
//PA4 - SPI NSS
//PA5 - SPI SCK
//PA6 - SPI MISO
//PA7 - SPI MOSI
  
//PA8 - ������, ��������� �������� � 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //�������� � 1
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
//PA9 - ������, ��������� �������� � 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //�������� � 1
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
//PA10 - ZV_OP - �������� �����������, ���������������� � 0
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //��� ��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOA,GPIO_Pin_10,0);
  GPIO_ResetBits(GPIOA, GPIO_Pin_10);
  
//PA11 - DPR - ������ ������� (�������������) ��������, ��� �������� 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOA,GPIO_Pin_11,0);
  GPIO_ResetBits(GPIOA, GPIO_Pin_11);
  
//PA12 - DVR - ������ ������� ��������, ������ �������� 10-1000 ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOA,GPIO_Pin_12,0);
  GPIO_ResetBits(GPIOA, GPIO_Pin_12);
  
  
  
//PB--------------------------------------------------------------------------
//PB0 - ADC - 420DVB - ��������� ������ ������� ��������
//PB1 - ADC - V_220  ���������� �� ���
  
//PB2 - PUSK_KON - ��������� �������� ����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOB,GPIO_Pin_2,0);
  GPIO_ResetBits(GPIOB, GPIO_Pin_2);
  
//PB5 - ������, ��������� �������� � 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //�������� � 1
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
//PB6 - K1_BKS_RAB - ������ ��� ������� ������ ��������� ������������ ��������� ���
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //��� ��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOB,GPIO_Pin_6,0);
  GPIO_ResetBits(GPIOB, GPIO_Pin_6);
  
//PB7 - K2_BKS_RAB - ������ ��� ������� ������ ��������� ������������ ��������� ���
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOB,GPIO_Pin_7,0);
  GPIO_ResetBits(GPIOB, GPIO_Pin_7);
  
//PB8 - K_VIK1 - ������ � ��������� ����������� 1 ������� ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOB,GPIO_Pin_8,0);
  GPIO_ResetBits(GPIOB, GPIO_Pin_8);
  
//PB9 - K_VIK2 - ������ � ��������� ����������� 2 ������� ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOB,GPIO_Pin_9,0);
  GPIO_ResetBits(GPIOB, GPIO_Pin_9);
  
//PB10 - STOP_KON - ���������� �������� ����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOB,GPIO_Pin_10,0);
  GPIO_ResetBits(GPIOB, GPIO_Pin_10);
  
//PB12 - AVAR_ASU - ������� �� ������, �������� ������� 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOB,GPIO_Pin_12,0);
  GPIO_ResetBits(GPIOB, GPIO_Pin_12);
  
//PB13 - RAB_ASU - ������ � ������, �������� ������� 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOB,GPIO_Pin_13,0);
  GPIO_ResetBits(GPIOB, GPIO_Pin_13);
  
//PB14 - ������, ��������� �������� � 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
//PB15 - ������, ��������� �������� � 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //�������� � 1
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  
  
//PC---------------------------------------------------------------
//P�0 - ADC - 420DVB - ��������� ������ ������� ��������
//P�1 - ADC - V_220  ���������� �� ���
  
//PC2 - PUSK_220  - �� ��� �� �������� ������� 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //��� ��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOC,GPIO_Pin_2,0);
  GPIO_ResetBits(GPIOC, GPIO_Pin_2);
  
//PC3 - PUSK_24 - �� ��� �� �������� ������� 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOC,GPIO_Pin_3,0);
  GPIO_ResetBits(GPIOC, GPIO_Pin_3);
  
//PC4 - ADC - TMP_ED1
//PC5 - ADC - TMP_P1
  
//PC6 - ������, ��������� �������� � 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  
//PC8 - CPU_LED - �������� ���� �� �����, �������� ������� 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOC,GPIO_Pin_8,0);
  GPIO_ResetBits(GPIOC, GPIO_Pin_8);
  
//PC10 - SV_OP - �������� ��������. ���������������� � 0
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOC,GPIO_Pin_10,0);
  GPIO_ResetBits(GPIOC, GPIO_Pin_10);
  
//PC11 - K2_BKS_ON - ������ ��������� ������� ����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOC,GPIO_Pin_11,0);
  GPIO_ResetBits(GPIOC, GPIO_Pin_11);
  
//PC12 - K1_BKS_ON - ������ ��������� ������� ����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOC,GPIO_Pin_12,0);
  GPIO_ResetBits(GPIOC, GPIO_Pin_12);

  
//PD-------------------------------------------------------------------------
//STOP_220 (PD2) - �� ��� �� �������� ������� 1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  //GPIO_WriteBit(GPIOD,GPIO_Pin_2,0);
  GPIO_ResetBits(GPIOD, GPIO_Pin_2);
  
}




void InitSpi(void)
{
  //SPI
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SPI_InitStructure;
  
  // ������������ ������ SPI1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//CLK, MOSI,MISO
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
// ����������� ���� SPI1 ��� ������ � ������ �������������� �������
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);       // SCK
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);       // MISO
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);       // MOSI
  
  
  SPI_DeInit(SPI1);
//��������� ��������� � ����������� SPI ������
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    //�������� � ��� �������
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                          // ����� - �������
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     // �������� �� 8 ���
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                           // ���������� (������� ������ = Low) �
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          // ���� ��������� ������� (�� ������� ����� SCK)
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             // ��������� ���������� ������� NSS ���������� (��� SSI)
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;    // ������������ SCK
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                    // ������ ������������ ������� ���
  SPI_InitStructure.SPI_CRCPolynomial = 7;                              // �� ���� CRC
  SPI_Init(SPI1, &SPI_InitStructure);                                   //����������� SPI1
    
  NVIC_EnableIRQ(SPI1_IRQn); //��������� ���������� �� SPI1
  SPI_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE); // ���������� SPI �� ������
//  SPI_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE); // ���������� SPI �� ��������
  
//  SPI_CalculateCRC(SPI1, DISABLE);
  
// �������� ������ SPI1....
  SPI_Cmd(SPI1, ENABLE);
  
//�������������/���������� (������/�������) ��� SSI (���������� �������� ������ NSS)
  SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}




void InitUart(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  
//������������ UART
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
//Init �����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //UP
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
  //RTS - �������, ���������� ������� ������ (0) � �������� (1)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOA, GPIO_Pin_1);
  
//��������� ���� � UART (������ ��� ��� UART1 ���� ���������
//                        ����������� �������, ���� �������)
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1 ,GPIO_AF_USART2); //RTS
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2 ,GPIO_AF_USART2); //TX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3 ,GPIO_AF_USART2); //RX

  
//Init ��� ������ UART
  USART_InitStructure.USART_BaudRate = UART_SPEED;    //����������� 2625000 � 16 ���
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_2;        // �� ��������� 2 ���� ����
  USART_InitStructure.USART_Parity = USART_Parity_No;   // ���� ��������
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ���� ���������� �������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  
//��� ���������� UART
  NVIC_EnableIRQ(USART2_IRQn);
//���������� �� ����� � ��������
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //����� ������ ��� ��� ������� ���������� ���������� �� ������
  USART_ITConfig(USART2, USART_IT_TC, ENABLE);
  
//��� UART
  USART_Cmd(USART2, ENABLE);
}



///ADC1 �.�. � STM32f401RE ���� ADC2, ADC3
void InitAdc(void)
{
  ADC_InitTypeDef ADC_InitStricture;
  ADC_CommonInitTypeDef ADC_CommonInitStruct;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); //�������� ������������ ���1
  
  ADC_DeInit(); //���������� ��������� ��� �� ���������
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;    //����� - ���������� ���� (���)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;       //�����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);          //�������� ���������
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  
  ADC_CommonInitStruct.ADC_Mode                 =ADC_Mode_Independent;
  ADC_CommonInitStruct.ADC_Prescaler            =ADC_Prescaler_Div2;    //������������ ������� ADC
  ADC_CommonInitStruct.ADC_DMAAccessMode        =ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStruct.ADC_TwoSamplingDelay     =ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStruct);

  
//��� ���
  ADC_InitStricture.ADC_Resolution = ADC_Resolution_12b;        //12 ���
  ADC_InitStricture.ADC_ScanConvMode = ENABLE;                  //��������� ������� (DISABLE 1 �����)
  ADC_InitStricture.ADC_ContinuousConvMode = ENABLE;             //����������� ���������������
  ADC_InitStricture.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None; //�� ������������ �������
  ADC_InitStricture.ADC_DataAlign = ADC_DataAlign_Right;        //������������ ���������� �� ������� ����
  ADC_InitStricture.ADC_NbrOfConversion = 6;                    //������������ 6 ������� ���
 
//������ ��������
  ADC_Init(ADC1, &ADC_InitStricture);
  
//��������� ������� ��� (ADC_SampleTime_3Cycles ��� ������ ��� ������, �� ��������)
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);  // TMP_P2
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_3Cycles);  // TMP_T
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 3, ADC_SampleTime_3Cycles); // TMP_ED1
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 4, ADC_SampleTime_3Cycles); // TMP_P1
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 5, ADC_SampleTime_3Cycles); // DVB
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 6, ADC_SampleTime_3Cycles); // V_220

  
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  
  ADC_DMACmd(ADC1, ENABLE);
  
  //ADC_ContinuousModeCmd(ADC1,ENABLE);
  
  ADC_Cmd(ADC1,ENABLE); //�������� ���
  ADC_SoftwareStartConv(ADC1);
  
}



// ��������� DMA ������ ��� �������� �� //�� ������� ADC1 � ������� ������ DMA
void InitDma (void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  DMA_InitTypeDef DMA_InitStructure;

  DMA_InitStructure.DMA_Channel = DMA_Channel_0;                //ADC1 ����� �� ������ ������ DMA
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;       //������ ������
  DMA_InitStructure.DMA_Memory0BaseAddr = (Uns)&g_RomData.AdcDataPointStruct;   //���� �����
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;               //�������� � ��������� � ������ (1 �� 3 ������� DMA)
  DMA_InitStructure.DMA_BufferSize = 6;                                 //������� ������
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;      // ��� ����� �� ���1
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;               // ����������������� ������ ������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;   // ������ ����� ������ 16 ���
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                       // ����������� �����
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                   // ������� ���������
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                // Fifo �� ������������
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;     // ��������� � Fifo
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;           // ���������� � �������� �������
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);   //�� ������� ADC1 � �������� ������ ������ DMA

  // DMA2_Stream0 enable 
  DMA_Cmd(DMA2_Stream0, ENABLE);
}