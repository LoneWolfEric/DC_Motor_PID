#include<bsp.h>
#include<encoder.h>


u16 encoder = 0;
u16 i = 0;
/**************************************************************************
�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ�ܶ�ʱ��2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PA �˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;  //�˿�����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //�����趨������ʼ��GPIOA

	//���ǳ���Ҫ������ͬʱ���ã�����ֿ����ã�������������������������������
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);   //����ط�ע��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);   //����ط�ע�� //GPIOA0��1����Ϊ��ʱ��2
	
//	/* Enable the TIM3 Update Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_Period = 65535; //�趨�������Զ���װֵ  //65535 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM���ϼ���  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	TIM_ICStructInit(&TIM_ICInitStructure); //���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 5;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//���и����ж�
	//Reset counter
	
	//	// Clear all pending interrupts
//	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	//Reset counter
//	//TIM2->CNT = 100;
	
	TIM_SetCounter(TIM2,0); //TIM2->CNT=0	 
	TIM_Cmd(TIM2, ENABLE); 
}

//����������
u16 TIM2_Encoder_Read(void)
{
	
//    TIM_SetCounter(TIM2,0);       //��������0
//        delay_ms(100);          //���ʱ�䣬�ɵ���
//    return TIM_GetCounter(TIM2);           //��������ת��
//                             //��¼���ر仯������һ��դ�񱻼�¼4�Σ�
	
	static u16 num = 0;
	
//	if(i > 0)
//	{
//		num = i * 0xffffffff;
//		num += TIM_GetCounter(TIM2); 
//		i = 0;
//	}
//	else
	
	num = TIM_GetCounter(TIM2); 
	
	//TIM_SetCounter(TIM2,0);      
	return num;
	
}

/**************************************************************************
�������ܣ�TIM2�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM2_IRQHandler(void)
{                                 
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
//		i ++;
//		printf("out:%d\r\n",i);
	}
}




//void Encoder_Init(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
//	TIM_ICInitTypeDef TIM_ICInitStructure;  
//	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

//	//GPIO_StructInit(&GPIO_InitStructure);
//	/* Configure PD.08,09 as encoder input */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);


//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);

//	/* Enable the TIM3 Update Interrupt */
//	//  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	//  NVIC_Init(&NVIC_InitStructure);

//	/* Timer configuration in Encoder mode */ 
//	//TIM_DeInit(TIM3);
//	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

//	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
//	TIM_TimeBaseStructure.TIM_Period = 60000; 
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

//	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//	TIM_ICStructInit(&TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_ICFilter = 5;
//	TIM_ICInit(TIM4, &TIM_ICInitStructure);

//	// Clear all pending interrupts
//	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
//	//Reset counter
//	//TIM2->CNT = 100;

//	// ENC_Clear_Speed_Buffer();
//	TIM_SetCounter(TIM2,0);
//	TIM_Cmd(TIM4, ENABLE); 
//}


