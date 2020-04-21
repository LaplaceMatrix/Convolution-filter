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
  * �������ƣ�LIMIT
  * ������������ֵ����
  * ��    �룺
              
  * ��    ����
  * ��    �أ�
  * ��    ע��
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
  * �������ƣ�UpdatePID
  * ��������������PID���ֵ
  * ��    �룺PIDInfo_t* pid��Ҫ�����PID�ṹ��ָ��
              float dt����λ����ʱ��
  * ��    ����void
  * ��    �أ�void
  * ��    ע��null
  *    
  *
******************************************************************************/
void UpdatePID(PIDInfo_t* pid, const float dt)
{
    float deriv;
    pid->Err = pid->desired - pid->measured + pid->offset; //��ǰ�Ƕ���ʵ�ʽǶȵ����
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
    pid->out = pid->kp * pid->Err + pid->ki * pid->integ + pid->kd * deriv;//PID���
    if(pid->OutLimitHigh != 0 && pid->OutLimitLow != 0)
    {
        pid->out = LIMIT(pid->out, pid->OutLimitLow, pid->OutLimitHigh);
    }
    pid->prevError = pid->Err;  
}
/******************************************************************************
  * �������ƣ�UpdatePID
  * ��������������PID���ֵ
  * ��    �룺PIDInfo_t* pid��Ҫ�����PID�ṹ��ָ��
              float dt����λ����ʱ��
  * ��    ����void
  * ��    �أ�void
  * ��    ע��null
  *    
  *
******************************************************************************/
u16 ctrl_cnt=0;
ctrl_command simulation_commander = stop;
//���������������ڼ������ѡȡ������ָ��
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
			TIM_SetTIM3Compare4(pid_motor.out*pwm_a);	//�޸ıȽ�ֵ���޸�ռ�ձ�
			TIM_SetTIM3Compare3(0);
		}
		else
		{
			TIM_SetTIM3Compare3(pid_motor.out*pwm_a);	//�޸ıȽ�ֵ���޸�ռ�ձ�
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
			TIM_SetTIM3Compare3(0);	//�޸ıȽ�ֵ���޸�ռ�ձ�
			TIM_SetTIM3Compare4(0);
		}
		else
		{
			TIM_SetTIM3Compare4(0);	//�޸ıȽ�ֵ���޸�ռ�ձ�
			TIM_SetTIM3Compare3(0);
		}
	}
	return ctrl_cnt;
}
/******************************************************************************
  * �������ƣ�UpdatePID
  * ��������������PID���ֵ
  * ��    �룺PIDInfo_t* pid��Ҫ�����PID�ṹ��ָ��
              float dt����λ����ʱ��
  * ��    ����void
  * ��    �أ�void
  * ��    ע��null
  *    
  *
******************************************************************************/
void ctrl_init(void)
{
	pid_init();
	TIM3_PWM_Init(500-1,200-1);     		//200M/200=1M�ļ���Ƶ�ʣ��Զ���װ��Ϊ500����ôPWMƵ��Ϊ1M/500=2kHZ
	//TIM2_Init(control_dt*10000-1,20000-1);      	//��ʱ��3��ʼ������ʱ��ʱ��Ϊ200M����Ƶϵ��Ϊ20000-1��
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




