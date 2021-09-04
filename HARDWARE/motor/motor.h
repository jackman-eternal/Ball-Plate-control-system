#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define BASE_one 1800							//定义rol方向上基础分量。
#define BASE_two 1800								//定义Pit方向上基础分量。舵臂水平时pwm波
#define BASE_three 1800

#define Rm_2 576 //Rm^2
#define D_2  6400//D^2


#define ROL_LIM 100									//定义ROL方向上限定值
#define PIT_LIM 100								//定义PIT方向上限定值


//extern volatile short Rol_Mod;			//Rol方向上修饰分量。
//extern volatile short Pit_Mod;			//Pit方向上修饰分量。

//void x_Motor_Conl(int pwm);			//声明电机控制函数。
//void y_Motor_Conl(int pwm);			//声明电机控制函数。
void Servor_1(float beta);
void Servor_2(float alfa);
void Servor_3(float alfa);
int angle2Pwm(int i,float angle);

#endif

