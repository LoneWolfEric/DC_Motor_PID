#ifndef __VISUALSCOPE_H__
#define __VISUALSCOPE_H__

#include "stm32f4xx.h"
#include "usart3.h"

//通过串口发送VisualScope识别的数据
void RS232_VisualScope	(USART_TypeDef* USARTx, u8*, u16 );

//通过串口发送VisualScope识别的数据
void VisualScope		(USART_TypeDef* USARTx,int16_t CH1,int16_t CH2,int16_t CH3,int16_t CH4);


void usart3_niming_report(u8 fun,u8*data,u8 len);
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart3_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);
#endif
