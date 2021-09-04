#include "sys.h"
#include "pid.h"
#include "usart.h"
#include "usart3.h" //传递x,y当前坐标
#include <math.h> 

volatile float x_p=0.012,x_i=0,x_d=0;
volatile float y_p=0.012,y_i=0,y_d=0;//pid parameter


volatile float x_lasterror = 0; 				//Error[-1]
volatile float x_sumerror = 0; 

volatile float y_lasterror = 0; 				//Error[-1]
volatile float y_sumerror = 0; 


volatile float x_setpos = 1878;				  //x方向设定值
volatile float y_setpos = 2050;				  //y方向设定值

//volatile float SerError_1 = 0;
//volatile float SerError_2 = 0;
//volatile float SerError_3 = 0;
//volatile float r3 = 1.73205;                   //根号3

//x方向上pid
float PID_x(void)
{
	float x_angl ;
	int index;
  float  Error1,LastError1;
	float Dval1,LastDval1,LastLastDval1;        //微分当前误差，上一次误差 ，上上次误差
	float PTerm1,ITerm1,DTerm1 ;                //比例，积分，微分值	
	Error1 = x_pos - x_setpos;  	      // 偏差
	Dval1 = Error1- LastError1; 					    // 当前微分误差
	
		if(Error1>50)
	{
		index = 0;
	}
	else if(Error1<30)
	{
		index = 1;
	}
	else
	{
		index = (50-Error1)/20;
	}  
	PTerm1 = (x_p*Error1)/10;                //比例值
//	ITerm += x_i*index*Error;            // 积分
	ITerm1 =0;            // 积分
	DTerm1 = x_d*(Dval1-LastDval1);
	// Calculate Output:
	x_angl = PTerm1 + ITerm1 + DTerm1 ;   //pid out	
	// save some calculations for later:
	LastError1=Error1;
	LastLastDval1=LastDval1;
	LastDval1= Dval1;
//	printf("Error 11:  *** %f\r\n",Error1);
//	printf("x_pid : *** %f  *** %f *** %f \r\n",PTerm1,ITerm1,DTerm1);
//	printf("x_angl: *** %f \r\n",x_angl);
	return  x_angl;            //返回计算值： x方向上倾斜角度

}


float PID_y(void)
{
	float  y_angl;
	int index;
  float  Error,LastError;
	float Dval,LastDval,LastLastDval;        //微分当前误差，上一次误差 ，上上次误差
	float PTerm,ITerm,DTerm ;                //比例，积分，微分值	
	
	Error = y_pos - y_setpos;

	Dval = Error- LastError; 					    // 当前微分误差
	if(Error>50)
	{
		index = 0;
	}
	else if(Error<30)
	{
		index = 1;
	}
	else
	{
		index = (50-Error)/20;
	}  
	PTerm = (y_p*Error)/10;                //比例值
	ITerm += y_i*index*Error;            // 积分
	DTerm = y_d*(Dval-LastDval);
	// Calculate Output:
	y_angl=PTerm + ITerm + DTerm ;
	
//	printf("Error:  *** %f\r\n",Error);
// save some calculations for later:
	LastError=Error;
	LastLastDval=LastDval;
	LastDval= Dval;
//	printf("y_DTerm: *** %f \r\n",DTerm);
//	printf("y_angl: *** %f \r\n",y_angl);  
	return y_angl;//返回计算值0  
	
}


//float GetError_1(int x_pos,int y_pos)                                                                                                                       
//{

//	SerError_1 = y_pos - y_setpos;
////	printf("SerError_1: %f *** \r\n",SerError_1);
//	return SerError_1;
//}

//float GetError_2(int x_pos,int y_pos)
//{

//	SerError_2 = (r3*(x_pos-x_setpos)-(y_pos - y_setpos))/2;
////	printf("SerError_2: %f *** \r\n",SerError_2);
//	return SerError_2;
//}

//float GetError_3(int x_pos,int y_pos)
//{
//	SerError_3 = (r3*(x_pos-x_setpos)+(y_pos - y_setpos))/2;
////	printf("SerError_3: %f *** \r\n",SerError_3);
//	return SerError_3;
//}
