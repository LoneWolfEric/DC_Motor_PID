#include "bsp.h"

int Deviation_Angle;
int step;
int set_pwm;

void ControlLoop(void)
{
	encoder = TIM2_Encoder_Read();
	set_pwm = Position_PID(encoder,15000);
	//printf("set_pwm：%d\r\n", set_pwm);
	Xianfu_Pwm();
	Set_Pwm_And_Direction(set_pwm);
	TIM_SetCompare2(TIM3, set_pwm);
	printf("encoder：%d\r\n", encoder);	
	VisualScope(USART3, encoder,0, 0, 0);
}

/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
int Position_PID (u16 Encoder,int Target)
{ 	
	 float Position_KP = 0.14, Position_KI = 0.0000000000009, Position_KD = 0.74;
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwm_And_Direction(int moto1)
{
			if(moto1>0)			Left=1,			Right=0;
			else 	          Left=0,			Right=1;
			set_pwm=myabs(moto1);
}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=100;    //===PWM满幅是7200 限制在7100
    if(set_pwm<-Amplitude) set_pwm=-Amplitude;	
		if(set_pwm>Amplitude)  set_pwm=Amplitude;	
}
