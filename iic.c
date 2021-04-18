/*
  ����˵��: IIC������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨ 8051��12MHz
  ��    ��: 2011-8-9
*/

#include <STC15F2K60S2.H>
#include "intrins.h"
#include "iic.h"

#define DELAY_TIME 40		//�޸ģ���5�޸�Ϊ40

#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//�������Ŷ���
sbit SDA = P2^1;  /* ������ */
sbit SCL = P2^0;  /* ʱ���� */

void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}
//������������
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//����ֹͣ����
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//����Ӧ��
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					// 0��Ӧ��1����Ӧ��
    IIC_Delay(DELAY_TIME);
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SCL = 0; 
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//�ȴ�Ӧ��
bit IIC_WaitAck(void)
{
    bit ackbit;
	
    SCL  = 1;
    IIC_Delay(DELAY_TIME);
    ackbit = SDA;
    SCL = 0;
    IIC_Delay(DELAY_TIME);
    return ackbit;
}

//ͨ��I2C���߷�������
void IIC_SendByte(unsigned char byt)
{
    unsigned char i;

    for(i=0; i<8; i++)
    {
        SCL  = 0;
        IIC_Delay(DELAY_TIME);
        if(byt & 0x80) SDA  = 1;
        else SDA  = 0;
        IIC_Delay(DELAY_TIME);
        SCL = 1;
        byt <<= 1;
        IIC_Delay(DELAY_TIME);
    }
    SCL  = 0;  
}

//��I2C�����Ͻ�������
unsigned char IIC_RecByte(void)
{
    unsigned char i, da;
    for(i=0; i<8; i++)
    {   
    	SCL = 1;
	IIC_Delay(DELAY_TIME);
	da <<= 1;
	if(SDA) da |= 1;
	SCL = 0;
	IIC_Delay(DELAY_TIME);
    }
    return da;    
}


/*************************����Ϊ�Լ���ӵ�************************************/

unsigned char PCF8591_Get_ADC(unsigned char adc_address)
{
	unsigned char ADC_data;
	IIC_Start();			    //IIC��ʼ�ź�
	IIC_SendByte(0x90);			//����Ѱַ������дģʽ
	IIC_WaitAck();      		//�ȴ��ӻ�Ӧ����Ӧ
    IIC_SendByte(adc_address);  //дͨ���ţ�����00,01,02,03
	IIC_WaitAck();      		//�ȴ��ӻ�Ӧ����Ӧ
    IIC_Stop();         		//ֹͣIIC
	
	IIC_Start();                //IIC��ʼ�ź�
	IIC_SendByte(0x91);    		//����Ѱַ�������ģʽ
	IIC_WaitAck();              //�ȴ��ӻ�Ӧ����Ӧ
	ADC_data = IIC_RecByte();  		//��һ���ֽ�
	IIC_SendAck(0);         	//������ӻ�������Ӧ����Ӧ,�ͷ�SDA
	IIC_Stop();         		//ֹͣIIC

	return ADC_data;			//����һ��0~255֮���AD���
}

void adc_set(unsigned char dat) //DAC���
{
	EA=0;
	IIC_Start(); 
	IIC_SendByte(0x90); // ��ַ+д
	IIC_WaitAck();
	IIC_SendByte(0x40);// DAC���ģʽ
	IIC_WaitAck();
	IIC_SendByte(dat);  ;//DACд������
	IIC_WaitAck();
	IIC_Stop();
	EA=1;
}

void AT24C02_write(unsigned char add, unsigned char dat)//AT24C02д����
{
	EA=0;
	IIC_Start();
	IIC_SendByte(SlaveAddrW);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
	EA=1;
	IIC_Delay(200);//�ص�
}

unsigned char AT24C02_read(add)//AT24C02������
{
	unsigned char dat;
	EA=0;
	IIC_Start();
	IIC_SendByte(SlaveAddrW);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(SlaveAddrR);
	IIC_WaitAck();
	dat=IIC_RecByte();
	IIC_SendAck(0);
	IIC_Stop();
	EA=1;
	return dat;
}