#include<bsp.h>
#include<encoder.h>


u16 encoder = 0;
u16 i = 0;
/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器2的时钟
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PA 端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;  //端口配置
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //根据设定参数初始化GPIOA

	//这点非常重要，不能同时复用，必须分开复用！！！！！！！！！！！！！！！！
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);   //这个地方注意
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);   //这个地方注意 //GPIOA0、1复用为定时器2
	
//	/* Enable the TIM3 Update Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
	TIM_TimeBaseStructure.TIM_Period = 65535; //设定计数器自动重装值  //65535 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure); //将结构体中的内容缺省输入
	TIM_ICInitStructure.TIM_ICFilter = 5;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//运行更新中断
	//Reset counter
	
	//	// Clear all pending interrupts
//	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	//Reset counter
//	//TIM2->CNT = 100;
	
	TIM_SetCounter(TIM2,0); //TIM2->CNT=0	 
	TIM_Cmd(TIM2, ENABLE); 
}

//读计数个数
u16 TIM2_Encoder_Read(void)
{
	
//    TIM_SetCounter(TIM2,0);       //计数器清0
//        delay_ms(100);          //检测时间，可调节
//    return TIM_GetCounter(TIM2);           //数据类型转换
//                             //记录边沿变化次数（一个栅格被记录4次）
	
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
函数功能：TIM2中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM2_IRQHandler(void)
{                                 
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
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


