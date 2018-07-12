#ifndef _ENCODER_H
#define _ENCODER_H
#include<sys.h>

#define DIR PCout(8)
#define ENA PCout(6)


void Encoder_Init_TIM2(void);
u16 TIM2_Encoder_Read(void);
void TIM2_IRQHandler(void);



#endif
