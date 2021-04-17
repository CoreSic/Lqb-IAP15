#ifndef __IIC_H
#define __IIC_H

void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 


void PCF8591_ADC_Init(unsigned char adc_address);
unsigned char adc_pcf8591(void);
unsigned char PCF8591_Get_ADC(unsigned char adc_address);

#endif