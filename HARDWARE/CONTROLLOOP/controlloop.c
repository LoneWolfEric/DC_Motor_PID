#include "bsp.h"

int Deviation_Angle;
int step;
int set_pwm;

void ControlLoop(void)
{
	encoder = TIM2_Encoder_Read();
	set_pwm = Position_PID(encoder,15000);
	//printf("set_pwm��%d\r\n", set_pwm);
	Xianfu_Pwm();
	Set_Pwm_And_Direction(set_pwm);
	TIM_SetCompare2(TIM3, set_pwm);
	printf("encoder��%d\r\n", encoder);	
	VisualScope(USART3, encoder,0, 0, 0);
}

/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
int Position_PID (u16 Encoder,int Target)
{ 	
	 float Position_KP = 0.14, Position_KI = 0.0000000000009, Position_KD = 0.74;
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm_And_Direction(int moto1)
{
			if(moto1>0)			Left=1,			Right=0;
			else 	          Left=0,			Right=1;
			set_pwm=myabs(moto1);
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=100;    //===PWM������7200 ������7100
    if(set_pwm<-Amplitude) set_pwm=-Amplitude;	
		if(set_pwm>Amplitude)  set_pwm=Amplitude;	
}
