#include "sys.h"
#include "timer.h"
#include "motor.h"
#include "usart3.h"
#include "usart.h"
#include "pid.h"
#include <math.h> 
#include <stdlib.h> 

volatile short Rol_Mod = 0;
volatile short Pit_Mod = 0;
//parameter  set
int base = 130;      //平板边长
int Rm = 24;         //舵臂长度
int D = 80;          //连杆长度
float  Lb = 67.25;   //舵机旋转点到底板中心点之间的距离
float  Lp = 74.75;   //平板角点到平板中心点之间的距离
float  r3 = 1.732;
float  h = 90;

int max_one = BASE_one + 80;
int max_two = BASE_two + 50;
int max_three = BASE_three + 50;

int min_one = BASE_one - 100; //1920
int min_two = BASE_two - 50;
int min_three = BASE_three - 50;

//三个角点坐标的设定
float x1 = 0      , y1 = -67.25 , z1 = 0;
float x2 = 64.734 , y2 = 33.625 , z2 = 0;
float x3 = -64.734, y3 = 33.625 , z3 = 0;

float x1_p,y1_p,z1_p;
float x2_p,y2_p,z2_p;
float x3_p,y3_p,z3_p;

//通过求运动学反解的方式推到出舵机相应摆角

void Servor_1(float beta)
{
	int pwm1;
	double a,b,c,a_2,b_2;
	double angleOut1,A1,B1;
	double y_delta,z_delta2;
	x1_p = 0 ;
	y1_p = -Lp*cos(beta);
	z1_p = h+Lp*sin(beta);
	y_delta = y1_p-y1;
	z_delta2 = (z1_p-z1)*(z1_p-z1);  // 平方
	
	a = 48*z1_p;          //2*Rm*z1_p;
	b = 48*y_delta;       // 2*Rm*y_delta;
	c = (y_delta*y_delta)+(z_delta2)+Rm_2-D_2; // 4.9残留问题 公式变量简化 提高运算速度
	a_2 = a * a ;//a^2
	b_2 = b * b ;//b^2
	
	A1 = c/(sqrt(a_2+b_2));
	B1 = b/a;
	angleOut1 = asin(A1) - atan(B1);
	angleOut1 = angleOut1 - 0.5352;    //0.6512;
//	printf("beta :  %f",beta);
//	printf("a:  %f  b:  %f   c:   %f",a,b,c);
//	printf("A1:  %f  B1:  %f",A1,B1);
//	printf("angleOut:  %f \r\n",angleOut1);
	pwm1 = angle2Pwm(1,angleOut1);
	printf("pwm1:  %d \r\n",pwm1);
//	

	if(pwm1 > max_one)     //超过1250最大
	{
		pwm1 = max_one;  //1250
	}
	else if(pwm1 < min_one)
	{
		pwm1 = min_one;							//对pwm限幅，限定值在头文件定义  //1150
	}
	
		TIM_SetCompare2(TIM3,pwm1);							//赋值给TIMER3-CCR3
//	
}
//Pit方向上电机控制
void Servor_2(float alfa)
{
	int pwm2,pwm3;
	double a2,b2,c2;
	double angleOut2,angleOut3,A2,B2;
	double x_delta,y_delta,z_delta, x_delta2 ,y_delta2 , z_delta2;
	x2_p = base*cos(alfa)/2 ;
	y2_p = 37.375;                                 //   Lp/2
	z2_p = h+(base*sin(alfa)/2);
	x_delta = x2_p - x2 ;
	y_delta = y2_p - y2 ;
	z_delta = z2_p - z2 ;
	x_delta2 = x_delta * x_delta;
	y_delta2 = y_delta * y_delta;
	z_delta2 = z_delta * z_delta;
	
	a2 = 48*z2_p;                                  //2*Rm*z2_p;
	b2 = 20.784*x_delta+24*y_delta;       //2*Rm*((x2_p-x2)*0.866+(y2_p-y2)/2);
	c2 = (x_delta2)+(y_delta2)+(z_delta2)+Rm_2-D_2;
	A2 = c2/(sqrt(a2*a2+b2*b2));
	B2 = b2/a2; 
	angleOut2 = asin(A2) - atan(B2)- 0.5352;
	angleOut3 = -angleOut2;
//	printf("a2:  %f  b2:  %f   c2:   %f\r\n",a2,b2,c2);
//	printf("A2:  %f  B2:  %f\r\n",A2,B2);
//	printf("angleOut2:  %f  \r\n",angleOut2);
  pwm2 = angle2Pwm(2,angleOut2);
	pwm3 = angle2Pwm(3,angleOut3);
		printf("pwm2:  %d \r\n",pwm2);
			printf("pwm3:  %d \r\n",pwm3);
		if(pwm2 > max_two)     //超过1250最大
	{
		pwm2 = max_two;  //1250
	}
	else if(pwm2 < min_two)
	{
		pwm2 = min_two;							//对pwm限幅，限定值在头文件定义  //1150
	}
	if(pwm3 > max_three)     //超过1250最大
	{
		pwm3 = max_three;  //1250
	}
	else if(pwm3 < min_three)
	{
		pwm3 = min_three;							//对pwm限幅，限定值在头文件定义  //1150
	}

	TIM_SetCompare3(TIM3,pwm2);
	TIM_SetCompare4(TIM3,pwm3);
////   
}

void Servor_3(float alfa)
{
	int pwm3;
	double a3,b3,c3;
	double angleOut3,A3,B3;
	x3_p = -base*cos(alfa)/2 ;
	y3_p = 37.375;                    //   Lp/2
	z3_p = h - (base*sin(alfa)/2);
	a3 = 2*Rm*z3_p;
	b3 = 2*Rm*( -(x3_p-x3)*0.866 + (y3_p-y3)/2 );
	c3 = ((x3_p-x3)*(x3_p-x3))+((y3_p-y3)*(y3_p-y3))+((z3_p-z3)*(z3_p-z3)) + Rm_2 - D_2;
	A3 = c3/(sqrt(a3*a3+b3*b3));
	B3 = b3/a3;
	angleOut3 = asin(A3) - atan(B3)- 0.5352;
//	printf("angleOut3:  %f  \r\n",angleOut3);
	pwm3=angle2Pwm(3,angleOut3);
	
	if(pwm3 > max_three)     //超过1250最大
	{
		pwm3 = max_three;  //1250
	}
	else if(pwm3 < min_three)
	{
		pwm3 = min_three;							//对pwm限幅，限定值在头文件定义  //1150
	}

	TIM_SetCompare4(TIM3,pwm3);
//	}
//		TIM_SetCompare4(TIM3,pwm3);
//   
}

int angle2Pwm(int i,float angle)
{
	int pwm ;

//	angle = angle *57.3; //转化为弧度  制
	switch(i)
	{
		case 1:
			pwm = (int)(angle*636.03 + BASE_one);
		break;
		case 2:
			pwm = (int)(BASE_two - angle*636.03 );
		break;
		case 3:
			pwm = (int)(BASE_three - angle*636.03 ) ;
		break;
	}
	
	return pwm; 
}

