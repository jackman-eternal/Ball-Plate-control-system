#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "usart3.h" 
#include "pid.h"
#include "motor.h"
//#include "dma.h"
/************************************************
 板球系统代码5.0版本
 采用三个pid分别控制三个舵机
 将误差进行分配到三个舵机上
 2019-2-24
 
************************************************/


 int main(void)
 {		
 	//u16 led0pwmval=0;

//	u8 dir=1;	
	 int res;
	u16 len;
	u16 t; 

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	usart3_init(9600);
// 	LED_Init();			     //LED端口初始化
 	TIM3_PWM_Init(8000-1,71);	 //不分频。PWM频率=125hz 20ms   250-2500
//	 TIM3_PWM_Init(9999,143);
	TIM4_Int_Init(9999,71);  //采样周期  10ms
//	TIM4_Int_Init(9999,143);  //采样周期  50ms
	 delay_ms(500);
	TIM_SetCompare2(TIM3,1800);	 
	 delay_ms(500);
	TIM_SetCompare3(TIM3,1800);
//	 delay_ms(500);
  TIM_SetCompare4(TIM3,1800);
	 
   	while(1)
	{		
    		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
		 
			
			for(t=0;t<len;t++)
			{
				
				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}

			 res =(USART_RX_BUF[0]-0x30)*1000+(USART_RX_BUF[1]-0x30)*100+(USART_RX_BUF[2]-0x30)*10+(USART_RX_BUF[3]-0x30);
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
			
		}
//		
//		printf("%d  %d\r\n",x_pos,y_pos);
//			GPIO_SetBits(GPIOA,GPIO_Pin_2);   //LED1对应引脚GPIOE.5拉高，灭 等同LED1=1;
//			delay_ms(300);  		   //延时300ms
//			GPIO_ResetBits(GPIOA,GPIO_Pin_2); //LED1对应引脚GPIOE.5拉低，亮 等同LED1=0;
//			delay_ms(300);                     //延时300ms
//		TIM_SetCompare1(TIM3,80);
	
//		TIM_SetCompare1(TIM3,400);
//		TIM_SetCompare2(TIM3,res);
//	  TIM_SetCompare3(TIM3,res);
//  	  TIM_SetCompare4(TIM3,res);                                            
	}	
}	

//		delay_ms(1000);
//		TIM_SetCompare2(TIM3,499.5);
		//270-> 174.125
		//0-> 194.025

//设定199，7199时
//025->2.5ms
//005->0.5ms

//设定9999,143
//0250->0.5ms
//1250->2.5ms
