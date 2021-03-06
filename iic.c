/*
  程序说明: IIC总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051，12MHz
  日    期: 2011-8-9
*/

#include <STC15F2K60S2.H>
#include "intrins.h"
#include "iic.h"

#define DELAY_TIME 40		//修改：将5修改为40

#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}
//总线启动条件
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//发送应答
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					// 0：应答，1：非应答
    IIC_Delay(DELAY_TIME);
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SCL = 0; 
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//等待应答
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

//通过I2C总线发送数据
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

//从I2C总线上接收数据
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


/*************************以下为自己添加的************************************/

unsigned char PCF8591_Get_ADC(unsigned char adc_address)
{
	unsigned char ADC_data;
	IIC_Start();			    //IIC起始信号
	IIC_SendByte(0x90);			//总线寻址，进入写模式
	IIC_WaitAck();      		//等待从机应答响应
    IIC_SendByte(adc_address);  //写通道号，可以00,01,02,03
	IIC_WaitAck();      		//等待从机应答响应
    IIC_Stop();         		//停止IIC
	
	IIC_Start();                //IIC起始信号
	IIC_SendByte(0x91);    		//总线寻址，进入读模式
	IIC_WaitAck();              //等待从机应答响应
	ADC_data = IIC_RecByte();  		//读一个字节
	IIC_SendAck(0);         	//主机向从机作出非应答响应,释放SDA
	IIC_Stop();         		//停止IIC

	return ADC_data;			//返回一个0~255之间的AD结果
}

void adc_set(unsigned char dat) //DAC输出
{
	EA=0;
	IIC_Start(); 
	IIC_SendByte(0x90); // 地址+写
	IIC_WaitAck();
	IIC_SendByte(0x40);// DAC输出模式
	IIC_WaitAck();
	IIC_SendByte(dat);  ;//DAC写入数据
	IIC_WaitAck();
	IIC_Stop();
	EA=1;
}

void AT24C02_write(unsigned char add, unsigned char dat)//AT24C02写数据
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
	IIC_Delay(200);//重点
}

unsigned char AT24C02_read(add)//AT24C02读数据
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