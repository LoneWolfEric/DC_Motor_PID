#include "visualscope.h"

/**********************************************************************
*���ܣ�ͨ��������visualscope����ʾ����������
*���룺��Ӧ�Ĵ��ڡ������4������
***********************************************************************/


//У�����ͣ�ֻ�ܶ���һ������Ҫ����λ��������ͬ
//#define CHECK_CRC_16
#define CHECK_SUM

#ifdef CHECK_CRC_16 //VisualScope��CRC16У��
static u16 RS232_VisualScope_CRC16( u8 *Array, u16 Len )	//CRCУ��
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

#ifdef CHECK_SUM	//VisualScope�����У��
static u8 RS232_VisualScope_CHKSUM(u8 *Array,u16 Len)
{
    u8 sum=0;
    u8 i=0;
    for(i=0; i<Len; i++)
        sum+=Array[i];
    return sum;
}
#endif


//ͨ�����ڷ���VisualScopeʶ�������
void VisualScope(USART_TypeDef* USARTx,int16_t CH1,int16_t CH2,int16_t CH3,int16_t CH4)	//ͨ��������ʾ�ĸ�ͨ���Ĳ���
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

#ifdef CHECK_CRC_16 //VisualScope��CRC16У��
    Temp = RS232_VisualScope_CRC16(Buffer, 8);
    Buffer[8] = Temp&0x00ff;
    Buffer[9] = (Temp&0xff00)>>8;
    for(i=0; i<10; i++)
    {
        USART_SendData(USARTx, Buffer[i]);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
#endif

#ifdef CHECK_SUM	//VisualScope�����У��
    Temp = RS232_VisualScope_CHKSUM(Buffer, 8);
    Buffer[8] = Temp&0x00ff;

    for(i=0; i<9; i++)
    {
        USART_SendData(USARTx, Buffer[i]);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
#endif
}

//ͨ�����ڷ���VisualScopeʶ�������
void RS232_VisualScope( USART_TypeDef* USARTx, u8 *pWord, u16 Len )	//ͨ�����ڷ���VisualScopeʶ�������
{
    u8 i = 0;
    u16 Temp = 0;

#ifdef CHECK_CRC_16 //VisualScope��CRC16У��
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

#ifdef CHECK_SUM	//VisualScope�����У��
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



//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart3_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++)USART3_SendChar(send_buf[i]);	//�������ݵ�����1 
}
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
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
	usart3_niming_report(0XA1,tbuf,12);//�Զ���֡,0XA1
}	
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart3_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
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
	usart3_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
} 
