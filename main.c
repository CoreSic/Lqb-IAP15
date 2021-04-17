//https://github.com/CoreSic/Lqb-IAP15
//��Ŀ���ƣ����ű�����-��Ƭ����demo
//���ߣ�coresic
//ת����ע������

#include <STC15F2K60S2.H>
#include "onewire.h"
#include "ds1302.h"
#include "iic.h"

#define uchar unsigned char
#define uint unsigned int
	
uchar WEI[8];//�������ʾֵ����
uchar DUAN[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};//����ܶ���
uchar index;//�������ʾλ��
float wendu;//�¶�����
uint count;//��ʱ������ֵ
uchar hour, minute, second;//ʱ����
uchar ADC_data;//ADC����

void Enable38(uchar num)//38������
{
	P2&=0x1f;
	P2|=num<<5;
}

void Timer0_Init()//��ʱ��0��ʼ��
{
	TMOD=0x01;
	TH0=-9;
	TL1=0;
	TR0=1;
	ET0=1;
	EA=1;
}

void Display_init()//�������ʾ��ʼ��
{
	WEI[0]=DUAN[0];
	WEI[1]=DUAN[0];
	WEI[2]=DUAN[0];
	WEI[3]=DUAN[0];
	WEI[4]=DUAN[0];
	WEI[5]=DUAN[0];
	WEI[6]=DUAN[0];
	WEI[7]=DUAN[0];
}

void Display_wendu()//�¶���ʾ
{
	uint wendu_num=wendu*10;
	WEI[0]=DUAN[wendu_num/100];
	WEI[1]=DUAN[wendu_num/10%10]&0x7f;
	WEI[2]=DUAN[wendu_num%10];
}

void Display_date()//ʱ����ʾ
{
	second=Read_Ds1302_Byte(0x81);//��ȡ��
	minute=Read_Ds1302_Byte(0x83);//��ȡ��
	hour=Read_Ds1302_Byte(0x85);//��ȡʱ
	
	WEI[0]=DUAN[hour/10];
	WEI[1]=DUAN[hour%10];
	
	WEI[2]=~0x40;
	
	WEI[3]=DUAN[minute/10];
	WEI[4]=DUAN[minute%10];
	
	WEI[5]=~0x40;
	
	WEI[6]=DUAN[second/10];
	WEI[7]=DUAN[second%10];
}

void Display_ADC(unsigned char address_ADC)
{
	ADC_data=PCF8591_Get_ADC(address_ADC);
	WEI[0]=DUAN[ADC_data/100];
	WEI[1]=DUAN[ADC_data/10%10];
	WEI[2]=DUAN[ADC_data%10];
	WEI[3]=0xff;
	WEI[4]=0xff;
	WEI[5]=0xff;
	WEI[6]=0xff;
	WEI[7]=0xff;
}

void Display()//�������ʾ
{
	Enable38(6);
	P0=(0x01<<index);
	Enable38(7);
	P0=(WEI[index]);
	index++;
	index&=7;
}

void LED8_ON(uchar num)//��ˮ�ƿ�
{
	Enable38(4);
	if(num<=7)
	P0&=~(0x01<<num);
	else
	P0=0X00;
}

void LED8_OFF(uchar num)//��ˮ�ƹ�
{
	Enable38(4);
	if(num<=7)
	P0|=0x01<<num;
	else
	P0=0Xff;
}

void main()//������
{
	float temp_num;
	Display_init();//�������ʾ��ʼ��
	Timer0_Init();//��ʱ��0��ʼ��
	Init_DS1302();//DS1302��ʼ��
	LED8_OFF(8);//�ر�8��led
	while(1)
	{
		if(count>=10)
		{
			count=0;
			
			Display_ADC(0x01);//ADC������ʾ
			
//			Display_date();//ʱ����ʾ
			
//			temp_num=Ds18b20_Get_data();//��ȡ�¶�ֵ
//			if(temp_num<30)
//			wendu=temp_num;//�����¶�ֵ
//			Display_wendu();//�����¶���ʾ
		}
		
		
	}
}

void Timer0() interrupt 1//��ʱ��0�ж�
{
	TH0=-9;
	count++;
	Display();//�������ʾ����
}

