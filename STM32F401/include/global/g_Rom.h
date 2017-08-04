#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "std.h"


#define ADC_TEMP_DATA_ROM_SIZE 80  // ������ ������ ��� ��� ������������������
#define ADC_VOLT_DATA_ROM_SIZE 1   // ������ ������ ��� ��� ����������� ����������


//��� ��� �������� ������������� ���������� �� ��������� IEEE-754 (� ��������� �������), 
//��� ������ � ��� ���������� �������������� ����� � ������ ��������,
//� �������� ������ ����������� ��� �� ���������� ��������
typedef struct _TVibrDataStructure
{
  Uns X_VibrData;   // ������ ������������� � �������
  Uns Y_VibrData;   // ������ ������������� � �������
  Uns Z_VibrData;   // ������ ������������� � �������
  
  Float X_Vibr;   // ������������� �� ��� �
  Float Y_Vibr;   // ������������� �� ��� Y
  Float Z_Vibr;   // ������������� �� ��� Z
 
}TVibrDataStructure;



typedef struct _TTempStructure
{
  MdInt Temp_P2;  // ����������� ���������� 2
  MdInt Temp_T;   // ����������� �����������
  MdInt Temp_ED1; // ����������� ����������������
  MdInt Temp_P1;  // ����������� ���������� 1
  
}TTempStructure;


//��������� ������ � ���������� ��������
typedef struct _AdcDataPointStructure
{
  MdBits   TMP_P2;         //����������� ���������� 2
  MdBits   TMP_T;          //����������� ��������� �����������
  MdBits   TMP_ED1;        //����������� ���������
  MdBits   TMP_P1;         //����������� ���������� 1
  MdBits   A_DVB;          //��������� ������ � ������� ��������
  MdBits   V_220;          //���������� ������� 220 �
  
}AdcDataPointStructure;


//������� �������� ������ ������ ���������� �������
typedef struct _AdcDataAverageStructure
{
  MdBits   TMP_P2;         //����������� ���������� 2
  MdBits   TMP_T;          //����������� ��������� �����������
  MdBits   TMP_ED1;        //����������� ���������
  MdBits   TMP_P1;         //����������� ���������� 1
  MdBits   A_DVB;          //��������� ������ � ������� ��������
  MdBits   V_220;          //���������� ������� 220 �
  
}AdcDataAverageStructure;



typedef struct _AdcDataBufStructure
{
  MdBits   TMP_P2[ADC_TEMP_DATA_ROM_SIZE];      //����������� ���������� 2
  MdBits   TMP_T[ADC_TEMP_DATA_ROM_SIZE];       //����������� ��������� �����������
  MdBits   TMP_ED1[ADC_TEMP_DATA_ROM_SIZE];     //����������� ���������
  MdBits   TMP_P1[ADC_TEMP_DATA_ROM_SIZE];      //����������� ���������� 1
  MdBits   A_DVB[ADC_TEMP_DATA_ROM_SIZE];       //��������� ������ � ������� ��������
  MdBits   V_220[ADC_VOLT_DATA_ROM_SIZE];       //���������� ������� 220 �
  
}AdcDataBufStructure;

//��������� ����� ���
typedef struct _AdcChannelStateStructure
{
  Byte   TMP_P2;         //��������� ������ P2
  Byte   TMP_T;          //��������� ������ T
  Byte   TMP_ED1;        //��������� ������ ED1
  Byte   TMP_P1;         //��������� ������ P1
  Byte   A_DVB;          //��������� ������ DVB
  Byte   V_220;          //��������� ������ V220
  
}AdcChannelStateStructure;




// ������� ��������� ����� 1
typedef union _GPIO_StateUnion
{
  Uns all;
  struct
  {
    Uns  K1_BKS_RAB:1;     // 0 ������ ��� ������ ������ ��������� ������������ ��������� ���
    Uns  K2_BKS_RAB:1;     // 1 ������ ��� ������ ������ ��������� ������������ ��������� ���
    Uns  K1_BKS_ON:1;      // 2 ������ ��������� ������� ����������
    Uns  K2_BKS_ON:1;      // 3 ������ ��������� ������� ����������
    Uns  K_VIK1:1;         // 4 ������ � ��������� ����������� 1 ������� ��������
    Uns  K_VIK2:1;         // 5 ������ � ��������� ����������� 2 ������� ��������
    Uns  PUSK_KON:1;       // 6 ������ �� ��������� �������� ����������
    Uns  STOP_KON:1;       // 7 ������ �� ���������� �������� ����������
    Uns  ZV_OP:1;          // 8 �������� �����������, �������� ������� 1
    Uns  SV_OP:1;          // 9 �������� ��������, �������� ������� 1
    Uns  DPR:1;            // 10 ������ ������� (�������������) ��������, ��� �������� 1
    Uns  CPU_LED:1;        // 11 �������� ���� �� �����, �������� ������� 1
    Uns  Resv:4;           // 12-15 ������
  }bit;
}GPIO_StateUnion;


// ������� ��������� ����� ����� � ��� 
typedef union _GPIO_AsuStateUnion
{
  Uns all;
  struct
  {
    Uns  PUSK_220:1;       // 0 ������ ���������� �� ��� �� �������� ������� 1
    Uns  PUSK_24:1;        // 1 ������ ���������� �� ��� �� �������� ������� 1
    Uns  STOP_220:1;       // 2 ������ ���������� �� ��� �� �������� ������� 1
    Uns  STOP_24:1;        // 3 ������ ���������� �� ��� �� �������� ������� 1
    Uns  AVAR_ASU:1;       // 4 ������� �� ������ �� ���, �������� ������� 1
    Uns  RAB_ASU:1;        // 5 ������ � ������ �� ���, �������� ������� 1
    Uns  Resv:10;          // 6-15 ������
  }bit;
}GPIO_AsuStateUnion;



typedef struct _TGroupA
{
//������ ���������� �������
  AdcDataPointStructure      AdcDataPointStruct;        // ���������� �������� ������ � ���
  AdcDataBufStructure        AdcDataBufStruct;          // ��������� ����� ������ ���
  AdcDataAverageStructure    AdcDataAverageStruct;      // ������� �������� ������ ���
  AdcChannelStateStructure   AdcChannelState;           // ��������� ���������� �����
  
  TTempStructure             TempStruct;                // ������ � �����������
  
//������ �������� ������� (�������) 
  TVibrDataStructure   Vibr;          //�������� �������� � ������� �������� (RS-485)  
  Uns                  Friq_ED;       //������� �������� ���� ~ ������� ������� 
  
//������� ��������� �����
  GPIO_StateUnion GpioStateUnion;
  GPIO_AsuStateUnion GpioAsuStateUnion;
}TGroupA;


extern TGroupA g_RomData;


void InterfaceInit(void);

#endif