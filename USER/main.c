#include<bsp.h>

int main()
{
	u8 i = 0;
	bsp_Init();
//	Clockwise_Rotate(200);
//	delay_ms(3000);
//	Counterclockwise_Rotate(200);
	while(1)
	{		
		//Clockwise_Rotate(3200);
//		Angle = Get_Adc1_Average(ADC_Channel_5,5);
//		printf("Angle=%u\r\n",Angle);
//		printf("Deviation_Angle=%d\r\n",Deviation_Angle);
//		printf("step=%d\r\n",step);
//		i ++;
//		VisualScope(USART3, Angle, 0, 0, 0);
		if(i > 40)
		{
			led0 = ~led0;
			led1 = ~led1;
			i = 0;
		}
//		TIM_SetCompare2(TIM3, 10);
		delay_ms(30);
		//printf("a");
//		encoder = TIM2_Encoder_Read();
//	printf("encoder£º%d\r\n", encoder);	
	}
}
