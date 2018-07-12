#include "bsp.h"

void bsp_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);
	uart_init(115200);
	uart2_init(115200);
	USART3_Init(115200);
	Adc_Init();
	Encoder_Init_TIM2();
	LED_Init();
	TIM3_PWM_Init(100, 84 - 1);
	//IWDG_Init(3,300);
	//KEY_Init();
	Motor_direction_Init();
	
	
	TIM14_Init();				//��ʱ���жϳ�ʼ����Ҫ�ö�ʱ����ؿ�����
	
}
