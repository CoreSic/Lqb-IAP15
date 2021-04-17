#ifndef __IIC_H
#define __IIC_H

void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 

unsigned char PCF8591_Get_ADC(unsigned char adc_address);
void adc_set(unsigned char dat);//DAC输出
void AT24C02_write(unsigned char add, unsigned char dat);//AT24C02写数据
unsigned char AT24C02_read(add);//AT24C02读数据

#endif