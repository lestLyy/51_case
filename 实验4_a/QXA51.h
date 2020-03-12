#ifndef __QXA51_H__
#define __QXA51_H__

/*电机驱动IO定义*/
sbit IN1 = P1^2; //为1 左电机反转
sbit IN2 = P1^3; //为1 左电机正转
sbit IN3 = P1^6; //为1 右电机正转
sbit IN4 = P1^7; //为1 右电机反转
sbit EN1 = P1^4; //为1 左电机使能
sbit EN2 = P1^5; //为1 右电机使能
sbit left_led1 = P3^3;//左寻迹信号 为0 没有识别到黑线 为1识别到黑线
sbit right_led1 = P3^2;//右寻迹信号	为0 没有识别到黑线 为1识别到黑线
sbit left_led2 = P3^4;//左避障信号 为0 识别障碍物 为1没有识别到障碍物
sbit right_led2 = P3^5;//右避障信号	为0 识别障碍物 为1没有识别到障碍物


/*按键定义*/
sbit key_s2 = P3^0;
sbit key_s3 = P3^1;
sbit beep = P2^3;//蜂鸣器

#define left_motor_en		EN1 = 1	//左电机使能
#define right_motor_en		EN2 = 1	//右电机使能


#define left_motor_stops	IN1 = 0, IN2 = 0//左电机停止
#define right_motor_stops	IN3 = 0, IN4 = 0//右电机停止

#define left_motor_go		IN1 = 0, IN2 = 1//左电机正传
#define left_motor_back		IN1 = 1, IN2 = 0//左电机反转
#define right_motor_go		IN3 = 1, IN4 = 0//右电机正传
#define right_motor_back	IN3 = 0, IN4 = 1//右电机反转

#endif