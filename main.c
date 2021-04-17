//https://github.com/CoreSic/Lqb-IAP15
//项目名称：蓝桥杯大赛-单片机组demo
//作者：coresic
//转载请注明出处

#include <STC15F2K60S2.H>
#include "onewire.h"
#include "ds1302.h"

#define uchar unsigned char
#define uint unsigned int
	
uchar WEI[8];//数码管显示值缓存
uchar DUAN[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};//数码管段码
uchar index;//数码管显示位置
float wendu;//温度数据
uint count;//定时器计数值

void Enable38(uchar num)//38译码器
{
	P2&=0x1f;
	P2|=num<<5;
}

void Timer0_Init()//定时器0初始化
{
	TMOD=0x01;
	TH0=-9;
	TL1=0;
	TR0=1;
	ET0=1;
	EA=1;
}

void Display_init()//数码管显示初始化
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

void Display_wendu()//温度显示
{
	uint wendu_num=wendu*10;
	WEI[0]=DUAN[wendu_num/100];
	WEI[1]=DUAN[wendu_num/10%10]&0x7f;
	WEI[2]=DUAN[wendu_num%10];
}

void Display()//数码管显示
{
	Enable38(6);
	P0=(0x01<<index);
	Enable38(7);
	P0=(WEI[index]);
	index++;
	index&=7;
}

void LED8_ON(uchar num)//流水灯开
{
	Enable38(4);
	if(num<=7)
	P0&=~(0x01<<num);
	else
	P0=0X00;
}

void LED8_OFF(uchar num)//流水灯关
{
	Enable38(4);
	if(num<=7)
	P0|=0x01<<num;
	else
	P0=0Xff;
}

void main()//主函数
{
	float temp_num;
	Display_init();//数码管显示初始化
	Timer0_Init();//定时器0初始化
	LED8_OFF(8);//关闭8个led
	while(1)
	{
		if(count>=10)
		{
			count=0;
			temp_num=Ds18b20_Get_data();//获取温度值
			if(temp_num<30)
			wendu=temp_num;//更新温度值
			Display_wendu();//更新温度显示
		}
		
	}
}

void Timer0() interrupt 1//定时器0中断
{
	TH0=-9;
	count++;
	Display();//数码管显示函数
}

