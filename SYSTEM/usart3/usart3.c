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
//串口接收缓存区 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
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
	

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
	{	  
	USART_ClearITPendingBit(USART3,USART_IT_RXNE); //清楚中断标志位
		res =USART_ReceiveData(USART3);
   //USART_SendData(USART1,res); 
		PosHandle(res);
		}
//	else if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
//	{
//		temp=USART3->SR;  
//    temp=USART3->DR; //这里我们通过先读SR（状态寄存器）和DR（数据寄存器）来清USART_IT_IDLE标志 
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
			if((abs(x_value)>4000||abs(y_value)>4000)||(((abs(x_value-x_salve)>1500||abs(y_value-y_salve)>1500))&&flag ==1))  //数据异常则使用上一次数据
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

void position(int x_val,int y_val)        //函数功能：将触摸屏换算成xy坐标输出x_pos,y_pos
{
	int delt = 130;     //跳变误差
	int  m=0 , j=0;                //记录坐标跳变次数
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
////跳变数据处理
//		m = x_pos/260; 
//		j = y_pos/260;
//			
//		x_pos = x_pos - m*delt ;
//		y_pos = y_pos - j*delt ;
		
//		x_last=x_evr;
////		y_last=y_evr;
//		x_pos=x_pos/11;
//		y_pos=y_pos/8.5;         //转化为mm单位坐标
		
//		printf("x:  %f\r\n",x_pos);
//		printf("y:  %f\r\n",y_pos);
}



//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void usart3_init(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口3时钟使能

 	USART_DeInit(USART3);  //复位串口3
		 //USART3_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10
   
    //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11
	
	USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART3, &USART_InitStructure); //初始化串口	3
  

	USART_Cmd(USART3, ENABLE);                    //使能串口 
	
	//使能接收中断
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);  //开启空闲中断
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
 
	USART3_RX_STA=0;		//清零


}

