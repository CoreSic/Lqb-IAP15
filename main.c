//���ű�����-��Ƭ����demo
//���ߣ�coresic

#include <STC15F2K60S2.H>

#define uchar unsigned char
#define uint unsigned int
	
uchar WEI[8];//�������ʾֵ����
uchar DUAN[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};//����ܶ���
uchar index;//�������ʾλ��

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

void Display_init()//����ܳ�ʼ��
{
	WEI[0]=DUAN[0];
	WEI[1]=DUAN[1];
	WEI[2]=DUAN[2];
	WEI[3]=DUAN[3];
	WEI[4]=DUAN[4];
	WEI[5]=DUAN[5];
	WEI[6]=DUAN[6];
	WEI[7]=DUAN[7];
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
	Display_init();
	Timer0_Init();
	LED8_OFF(8);
	while(1)
	{
	
	}
}

void Timer0() interrupt 1//��ʱ��0�ж�
{
	TH0=-9;
	Display();
}

