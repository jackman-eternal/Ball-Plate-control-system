#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define BASE_one 1800							//����rol�����ϻ���������
#define BASE_two 1800								//����Pit�����ϻ������������ˮƽʱpwm��
#define BASE_three 1800

#define Rm_2 576 //Rm^2
#define D_2  6400//D^2


#define ROL_LIM 100									//����ROL�������޶�ֵ
#define PIT_LIM 100								//����PIT�������޶�ֵ


//extern volatile short Rol_Mod;			//Rol���������η�����
//extern volatile short Pit_Mod;			//Pit���������η�����

//void x_Motor_Conl(int pwm);			//����������ƺ�����
//void y_Motor_Conl(int pwm);			//����������ƺ�����
void Servor_1(float beta);
void Servor_2(float alfa);
void Servor_3(float alfa);
int angle2Pwm(int i,float angle);

#endif

