#include "delay.h"
#include "usart3.h"
#include "usart.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 
#include "timer.h"
#include <math.h> 
#include "dma.h"
#include <stdlib.h> 
//���ڽ��ջ����� 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART3_RX_STA=0;   	
 int x_value  ;
 int y_value  ;  
 int x_salve;
 int y_salve;
vu8 i=0;
 int flag = 0;
 
void USART3_IRQHandler(void)
{
	int res;
	

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
	{	  
	USART_ClearITPendingBit(USART3,USART_IT_RXNE); //����жϱ�־λ
		res =USART_ReceiveData(USART3);
   //USART_SendData(USART1,res); 
		PosHandle(res);
		}
//	else if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
//	{
//		temp=USART3->SR;  
//    temp=USART3->DR; //��������ͨ���ȶ�SR��״̬�Ĵ�������DR�����ݼĴ���������USART_IT_IDLE��־ 
//		temp = temp;
//		DMA_Cmd(DMA1_Channel3,DISABLE);
//		PosHandle();
//		DMA_Cmd(DMA1_Channel3,ENABLE);
//		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
//	}
    		
 }
	
void PosHandle(int data)
{

	int get[6];
	
	if(data == 0x81||data == 0x80)
		{
			USART3_RX_STA = 1;
			i=0;
		}
		else if (USART3_RX_STA==1)
		{	
			
			get[i]=data;
//			printf("%d\r\n",get[i]);
			i++;
			
		}
		 if(i==4)
		{
			x_value=(get[0]*256)+get[1];
			y_value=(get[2]*256)+get[3];
//			
			if((abs(x_value)>4000||abs(y_value)>4000)||(((abs(x_value-x_salve)>1500||abs(y_value-y_salve)>1500))&&flag ==1))  //�����쳣��ʹ����һ������
			{
//				printf("%d  %d\r\n",x_salve,y_salve);
//				printf("error--------\r\n");
				position(x_salve,y_salve);
			}
			else
			{
//				printf("%d  %d\r\n",x_value,y_value);
				position(x_value,y_value);
				x_salve=x_value;
			  y_salve=y_value;
				flag = 1;
			}
			
//			printf("%d  ////*****  %d\r\n",x_salve,y_salve);
//			i=0;
			USART3_RX_STA = 0;
			}
}
 
 


	int  m=0;
	int x_last;
	int y_last;

	float x_pos=0;
 	float y_pos=0;

void position(int x_val,int y_val)        //�������ܣ��������������xy�������x_pos,y_pos
{
	int delt = 130;     //�������
	int  m=0 , j=0;                //��¼�����������
	int x_evr=0;
	int y_evr=0;
//	int x_pos=0;
//	int y_pos=0;
	
//printf("%d\r\n",x_val);
		x_pos=x_val;
		y_pos=y_val;

//		x_evr=x_evr-2002;
//		y_evr=y_evr-2002;
//		if(x_evr<=-83)
//		{
//			x_pos=x_evr+83;
//		}
//		else 
//		{
//			x_pos=x_evr-45;
//		}
//		if(y_evr<=-83)
//		{
//			y_pos=y_evr+83;
//		}
//		else 
//		{
//			y_pos=y_evr-45;
//		}
////�������ݴ���
//		m = x_pos/260; 
//		j = y_pos/260;
//			
//		x_pos = x_pos - m*delt ;
//		y_pos = y_pos - j*delt ;
		
//		x_last=x_evr;
////		y_last=y_evr;
//		x_pos=x_pos/11;
//		y_pos=y_pos/8.5;         //ת��Ϊmm��λ����
		
//		printf("x:  %f\r\n",x_pos);
//		printf("y:  %f\r\n",y_pos);
}



//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void usart3_init(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����3ʱ��ʹ��

 	USART_DeInit(USART3);  //��λ����3
		 //USART3_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PB10
   
    //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11
	
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART3, &USART_InitStructure); //��ʼ������	3
  

	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
	
	//ʹ�ܽ����ж�
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);  //���������ж�
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
 
	USART3_RX_STA=0;		//����


}

