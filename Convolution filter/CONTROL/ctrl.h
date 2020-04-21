#ifndef _CTRL_H
#define _CTRL_H
#include "sys.h"

typedef struct
{
    float kp;           //< proportional gain
    float ki;           //< integral gain
    float kd;           //< derivative gain
    float out;
    float Err;
    float desired;     //< set point
    float measured;

    float Err_LimitHigh;
    float Err_LimitLow;
    
    float offset;      //
    float prevError;    //< previous error
    float integ;        //< integral

    float IntegLimitHigh;       //< integral limit
    float IntegLimitLow;

    float OutLimitHigh;
    float OutLimitLow;
}PIDInfo_t;
typedef enum
{
	ctrl = 0,
	stop = 1
}ctrl_command;


#define control_dt	 	0.02    //s
#define simulate_time		3//µ¥Î»Îªs
#define simulate_cnt    (int)(simulate_time/control_dt)
	

#define pwm_a 				5
#define encoder_lines 512*68/30

#define aim_rpm 			60
#define stop_rpm			0.01


void pid_init(void);
void pid_reset(void);
u16 ctrl_motor(ctrl_command command, const float dt, const float aim);
void ctrl_init(void);
void simulation_once(u16 cnt);











#endif

