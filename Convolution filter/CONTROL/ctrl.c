#include "ctrl.h"
#include "timer.h"
#include "exti.h"
#include "eigamath.h"
PIDInfo_t pid_motor;
void pid_init(void)
{
	pid_motor.kp = 1.0f;
  pid_motor.ki = 0.01f;
  pid_motor.kd = 0.01f;
  pid_motor.OutLimitHigh = 99;
  pid_motor.OutLimitLow = -99;
	pid_motor.IntegLimitHigh=200;
	pid_motor.Err_LimitLow=-200;
	
}
void pid_reset(void)
{
	pid_motor.kp = 1.0f;
  pid_motor.ki = 0.01f;
  pid_motor.kd = 0.01f;
  pid_motor.OutLimitHigh = 99;
  pid_motor.OutLimitLow = -99;
	pid_motor.out=0;
	pid_motor.Err=0;
	pid_motor.desired=0;
	pid_motor.measured=0;
	pid_motor.offset=0;
	pid_motor.prevError=0;
	pid_motor.integ=0;
}
/******************************************************************************
  * 函数名称：LIMIT
  * 函数描述：限值计算
  * 输    入：
              
  * 输    出：
  * 返    回：
  * 备    注：
  *    
  *
******************************************************************************/
float LIMIT(float buff,float min,float max)
{
  if(buff>min&&buff<max)return buff;
  else if(buff<min)return min;
  else if(buff>max)return max;
  else return buff;
}
/******************************************************************************
  * 函数名称：UpdatePID
  * 函数描述：计算PID相关值
  * 输    入：PIDInfo_t* pid：要计算的PID结构体指针
              float dt：单位运行时间
  * 输    出：void
  * 返    回：void
  * 备    注：null
  *    
  *
******************************************************************************/
void UpdatePID(PIDInfo_t* pid, const float dt)
{
    float deriv;
    pid->Err = pid->desired - pid->measured + pid->offset; //当前角度与实际角度的误差
    if(pid->Err_LimitHigh != 0 && pid->Err_LimitLow != 0)
    {
        pid->Err = LIMIT(pid->Err, pid->Err_LimitLow, pid->Err_LimitHigh);
    }
    pid->integ += pid->Err * dt;    
    if(pid->IntegLimitHigh != 0 && pid->IntegLimitLow != 0)
    {
        pid->integ = LIMIT(pid->integ, pid->IntegLimitLow, pid->IntegLimitHigh);
    }
    deriv = (pid->Err - pid->prevError)/dt;  
    pid->out = pid->kp * pid->Err + pid->ki * pid->integ + pid->kd * deriv;//PID输出
    if(pid->OutLimitHigh != 0 && pid->OutLimitLow != 0)
    {
        pid->out = LIMIT(pid->out, pid->OutLimitLow, pid->OutLimitHigh);
    }
    pid->prevError = pid->Err;  
}
/******************************************************************************
  * 函数名称：UpdatePID
  * 函数描述：计算PID相关值
  * 输    入：PIDInfo_t* pid：要计算的PID结构体指针
              float dt：单位运行时间
  * 输    出：void
  * 返    回：void
  * 备    注：null
  *    
  *
******************************************************************************/
u16 ctrl_cnt=0;
ctrl_command simulation_commander = stop;
//下面两个数组用于计算参数选取的最优指标
float speed_error_list[10];//[simulate_cnt+10];
float ctrl_output_list[10];//[simulate_cnt+10];
u16 ctrl_motor(ctrl_command command, const float dt, const float aim)
{
	ctrl_cnt++;
	if(command == ctrl)
	{
		pid_motor.desired = aim;
		pid_motor.measured = encoder_get();
		UpdatePID(&pid_motor, dt);
		if(pid_motor.out>0)
		{
			TIM_SetTIM3Compare4(pid_motor.out*pwm_a);	//修改比较值，修改占空比
			TIM_SetTIM3Compare3(0);
		}
		else
		{
			TIM_SetTIM3Compare3(pid_motor.out*pwm_a);	//修改比较值，修改占空比
			TIM_SetTIM3Compare4(0);
		}
		//speed_error_list[ctrl_cnt]=pid_motor.Err;
		//ctrl_output_list[ctrl_cnt]=pid_motor.out;
	}
	else
	{
		ctrl_cnt=0;
		pid_motor.desired = 0;
		pid_motor.measured = encoder_get();
		UpdatePID(&pid_motor, dt);
		if(pid_motor.out>0)
		{
			TIM_SetTIM3Compare3(0);	//修改比较值，修改占空比
			TIM_SetTIM3Compare4(0);
		}
		else
		{
			TIM_SetTIM3Compare4(0);	//修改比较值，修改占空比
			TIM_SetTIM3Compare3(0);
		}
	}
	return ctrl_cnt;
}
/******************************************************************************
  * 函数名称：UpdatePID
  * 函数描述：计算PID相关值
  * 输    入：PIDInfo_t* pid：要计算的PID结构体指针
              float dt：单位运行时间
  * 输    出：void
  * 返    回：void
  * 备    注：null
  *    
  *
******************************************************************************/
void ctrl_init(void)
{
	pid_init();
	TIM3_PWM_Init(500-1,200-1);     		//200M/200=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	//TIM2_Init(control_dt*10000-1,20000-1);      	//定时器3初始化，定时器时钟为200M，分频系数为20000-1，
}



extern u16 param_list[];
extern float fitness_list[];
void simulation_once(u16 cnt)
{
	pid_reset();
	if(param_sitch == param_switch_kp)pid_motor.kp = param_list[cnt]/param_a;
	else if(param_sitch == param_switch_ki)pid_motor.ki = param_list[cnt]/param_a;
	else pid_motor.kd = param_list[cnt]/param_a;
	
	simulation_commander = ctrl;
	while(1)
	{
		if(ctrl_cnt>simulate_cnt)simulation_commander = stop;
		if((ctrl_cnt>simulate_cnt)&&(ga_abs(pid_motor.measured)<stop_rpm))
		{
			ctrl_cnt=0;
			break;
		}
	}
	fitness_list[cnt]=caculate_fitness_once()/fitness_a;
}




