#ifndef __USART1_H__
#define __USART1_H__

#include "stm32f4xx.h"
#include "stdio.h"


#define USART_REC_LEN  			200  	//�����������ֽ��� 200
//extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u16 USART_RX_STA;         		//����״̬���




void USART3_Init(u32 bound);	
void USART3_SendChar(unsigned char b);
void USART3_SendString(uint8_t *buffer,uint16_t len);
//int fputc(int ch, FILE *f);
	
#endif


