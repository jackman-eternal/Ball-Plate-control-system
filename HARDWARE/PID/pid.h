#ifndef __PID_H
#define __PID_H
extern volatile float x_p,x_i,x_d;	//roll�����ϵ�pid����
extern volatile float y_p,y_i,y_d;	//pitch�����ϵ�pid����

//extern volatile float x_lasterror; 			//Error[-1]
//extern volatile float x_sumerror;  
extern volatile float x_setpos;				  //x�����趨ֵ
//extern volatile float x_curpos;					//x����ǰֵ

//extern volatile float y_lasterror; 			//Error[-1]
//extern volatile float y_sumerror;  
extern volatile float y_setpos;				  //x�����趨ֵ
//extern volatile float y_curpos;					//x����ǰֵ

//extern volatile float SerError_1;
//extern volatile float SerError_2;
//extern volatile float SerError_3;                //���������

//int PID_one(void);   //x����pid���ֵ
//int PID_y_Pos(float NextPoint);   //y����pid���ֵ
float PID_x(void);
float PID_y(void);


//float GetError_1(int x_pos,int y_pos);
//float GetError_2(int x_pos,int y_pos);
//float GetError_3(int x_pos,int y_pos);//��������������������

#endif
