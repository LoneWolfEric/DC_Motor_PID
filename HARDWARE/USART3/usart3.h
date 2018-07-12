#ifndef __USART1_H__
#define __USART1_H__

#include "stm32f4xx.h"
#include "stdio.h"


#define USART_REC_LEN  			200  	//定义最大接收字节数 200
//extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern u16 USART_RX_STA;         		//接收状态标记




void USART3_Init(u32 bound);	
void USART3_SendChar(unsigned char b);
void USART3_SendString(uint8_t *buffer,uint16_t len);
//int fputc(int ch, FILE *f);
	
#endif


