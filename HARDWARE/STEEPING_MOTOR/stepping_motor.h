#ifndef _PWM_H
#define _PWM_H
#include<sys.h>

#define Left PBout(8)
#define Right PBout(6)


void TIM3_PWM_Init(u16 arr,u16 psc);
void Motor_direction_Init(void);

//void Stepping_motor_init(void);
//void Clockwise_Rotate(int step_num);
//void Counterclockwise_Rotate(int step_num);




#endif
