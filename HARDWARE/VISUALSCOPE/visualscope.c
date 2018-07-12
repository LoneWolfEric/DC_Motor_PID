#include "visualscope.h"

/**********************************************************************
*功能：通过串口在visualscope上显示连续的数据
*输入：相应的串口、输出的4个变量
***********************************************************************/


//校验类型，只能定义一个，需要和上位机设置相同
//#define CHECK_CRC_16
#define CHECK_SUM

#ifdef CHECK_CRC_16 //VisualScope的CRC16校验
static u16 RS232_VisualScope_CRC16( u8 *Array, u16 Len )	//CRC校验
{
    u16 USART_IX, USART_IY, USART_CRC;

    USART_CRC = 0xffff;
    for(USART_IX=0; USART_IX<Len; USART_IX++)
    {
        USART_CRC = USART_CRC^(u16)(Array[USART_IX]);
        for(USART_IY=0; USART_IY<=7; USART_IY++)
        {
            if((USART_CRC&1)!=0)
                USART_CRC = (USART_CRC>>1)^0xA001;
            else
                USART_CRC = USART_CRC>>1;
        }
    }
    return(USART_CRC);
}
#endif

#ifdef CHECK_SUM	//VisualScope的求和校验
static u8 RS232_VisualScope_CHKSUM(u8 *Array,u16 Len)
{
    u8 sum=0;
    u8 i=0;
    for(i=0; i<Len; i++)
        sum+=Array[i];
    return sum;
}
#endif


//通过串口发送VisualScope识别的数据
void VisualScope(USART_TypeDef* USARTx,int16_t CH1,int16_t CH2,int16_t CH3,int16_t CH4)	//通过串口显示四个通道的波形
{
    u8 i = 0;
    uint8_t Buffer[10];
    uint16_t Temp=0;

    Buffer[0]=CH1&0xff;
    Buffer[1]=CH1>>8;
    Buffer[2]=CH2&0xff;;
    Buffer[3]=CH2>>8;
    Buffer[4]=CH3&0xff;;
    Buffer[5]=CH3>>8;
    Buffer[6]=CH4&0xff;;
    Buffer[7]=CH4>>8;

#ifdef CHECK_CRC_16 //VisualScope的CRC16校验
    Temp = RS232_VisualScope_CRC16(Buffer, 8);
    Buffer[8] = Temp&0x00ff;
    Buffer[9] = (Temp&0xff00)>>8;
    for(i=0; i<10; i++)
    {
        USART_SendData(USARTx, Buffer[i]);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
#endif

#ifdef CHECK_SUM	//VisualScope的求和校验
    Temp = RS232_VisualScope_CHKSUM(Buffer, 8);
    Buffer[8] = Temp&0x00ff;

    for(i=0; i<9; i++)
    {
        USART_SendData(USARTx, Buffer[i]);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
#endif
}

//通过串口发送VisualScope识别的数据
void RS232_VisualScope( USART_TypeDef* USARTx, u8 *pWord, u16 Len )	//通过串口发送VisualScope识别的数据
{
    u8 i = 0;
    u16 Temp = 0;

#ifdef CHECK_CRC_16 //VisualScope的CRC16校验
    Temp = RS232_VisualScope_CRC16(pWord, Len);
    pWord[8] = Temp&0x00ff;
    pWord[9] = (Temp&0xff00)>>8;

    for(i=0; i<10; i++)
    {
        USART_SendData(USARTx, (uint8_t)*pWord);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
        pWord++;
    }
#endif

#ifdef CHECK_SUM	//VisualScope的求和校验
    Temp = RS232_VisualScope_CHKSUM(pWord, Len);
    pWord[8] = Temp&0xff;

    for(i=0; i<9; i++)
    {
        USART_SendData(USARTx, (uint8_t)*pWord);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
        pWord++;
    }
#endif

}



//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart3_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)USART3_SendChar(send_buf[i]);	//发送数据到串口1 
}
//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart3_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
}	
//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart3_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart3_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
} 
