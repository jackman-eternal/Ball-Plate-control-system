#ifndef __PID_H
#define __PID_H
extern volatile float x_p,x_i,x_d;	//roll方向上的pid数据
extern volatile float y_p,y_i,y_d;	//pitch方向上的pid数据

//extern volatile float x_lasterror; 			//Error[-1]
//extern volatile float x_sumerror;  
extern volatile float x_setpos;				  //x方向设定值
//extern volatile float x_curpos;					//x方向当前值

//extern volatile float y_lasterror; 			//Error[-1]
//extern volatile float y_sumerror;  
extern volatile float y_setpos;				  //x方向设定值
//extern volatile float y_curpos;					//x方向当前值

//extern volatile float SerError_1;
//extern volatile float SerError_2;
//extern volatile float SerError_3;                //各方向误差

//int PID_one(void);   //x方向pid输出值
//int PID_y_Pos(float NextPoint);   //y方向pid输出值
float PID_x(void);
float PID_y(void);


//float GetError_1(int x_pos,int y_pos);
//float GetError_2(int x_pos,int y_pos);
//float GetError_3(int x_pos,int y_pos);//计算三个舵机分量的误差

#endif
