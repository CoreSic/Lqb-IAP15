//https://github.com/CoreSic/Lqb-IAP15
//项目名称：蓝桥杯大赛-单片机组demo
//作者：coresic
//转载请注明出处

#include <STC15F2K60S2.H>
#include "onewire.h"
#include "ds1302.h"
#include "iic.h"

#define uchar unsigned char
#define uint unsigned int
	
uchar WEI[8];//数码管显示值缓存
uchar DUAN[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};//数码管段码
uchar index;//数码管显示位置
float wendu;//温度数据
uint count;//定时器计数值
uchar hour, minute, second;//时分秒
uchar ADC_data;//ADC数据

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

void Display_date()//时间显示
{
	second=Read_Ds1302_Byte(0x81);//读取秒
	minute=Read_Ds1302_Byte(0x83);//读取分
	hour=Read_Ds1302_Byte(0x85);//读取时
	
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
	Init_DS1302();//DS1302初始化
	LED8_OFF(8);//关闭8个led
	while(1)
	{
		if(count>=10)
		{
			count=0;
			
			Display_ADC(0x01);//ADC数据显示
			
//			Display_date();//时间显示
			
//			temp_num=Ds18b20_Get_data();//获取温度值
//			if(temp_num<30)
//			wendu=temp_num;//更新温度值
//			Display_wendu();//更新温度显示
		}
		
		
	}
}

void Timer0() interrupt 1//定时器0中断
{
	TH0=-9;
	count++;
	Display();//数码管显示函数
}

