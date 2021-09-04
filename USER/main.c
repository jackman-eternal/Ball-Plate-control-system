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
 ����ϵͳ����5.0�汾
 ��������pid�ֱ�����������
 �������з��䵽���������
 2019-2-24
 
************************************************/


 int main(void)
 {		
 	//u16 led0pwmval=0;

//	u8 dir=1;	
	 int res;
	u16 len;
	u16 t; 

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	usart3_init(9600);
// 	LED_Init();			     //LED�˿ڳ�ʼ��
 	TIM3_PWM_Init(8000-1,71);	 //����Ƶ��PWMƵ��=125hz 20ms   250-2500
//	 TIM3_PWM_Init(9999,143);
	TIM4_Int_Init(9999,71);  //��������  10ms
//	TIM4_Int_Init(9999,143);  //��������  50ms
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
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		 
			
			for(t=0;t<len;t++)
			{
				
				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}

			 res =(USART_RX_BUF[0]-0x30)*1000+(USART_RX_BUF[1]-0x30)*100+(USART_RX_BUF[2]-0x30)*10+(USART_RX_BUF[3]-0x30);
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
			
		}
//		
//		printf("%d  %d\r\n",x_pos,y_pos);
//			GPIO_SetBits(GPIOA,GPIO_Pin_2);   //LED1��Ӧ����GPIOE.5���ߣ��� ��ͬLED1=1;
//			delay_ms(300);  		   //��ʱ300ms
//			GPIO_ResetBits(GPIOA,GPIO_Pin_2); //LED1��Ӧ����GPIOE.5���ͣ��� ��ͬLED1=0;
//			delay_ms(300);                     //��ʱ300ms
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

//�趨199��7199ʱ
//025->2.5ms
//005->0.5ms

//�趨9999,143
//0250->0.5ms
//1250->2.5ms
