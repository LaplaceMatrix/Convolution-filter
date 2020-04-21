#include "eigamath.h"
#include "ctrl.h"
#include "rng.h"
#include "ctrl.h"
extern PIDInfo_t pid_motor;

u16 param_list[POPULATION];
float fitness_list[POPULATION];
u16 fitness_list_line;
ga_t ga;
void ga_param_init(void)
{
	ga.population = POPULATION;
	ga.generation = GENERATION;
	ga.p_cross = P_CROSS;
	ga.p_mut = P_MUT;
	ga.eliminate = ELIMAINATE;
}
void list_init(void)
{
	int i=0;
	if(param_sitch == param_switch_kp)
	{
		for(i=0;i<POPULATION;i++)
			param_list[i]=pid_motor.kp*param_a;
			fitness_list[i] = 0;
	}
	else if(param_sitch == param_switch_ki)
	{
		for(i=0;i<POPULATION;i++)
			param_list[i]=pid_motor.ki*param_a;
			fitness_list[i] = 0;
	}
	else if(param_sitch == param_switch_kd)
	{
		for(i=0;i<POPULATION;i++)
			param_list[i]=pid_motor.kd*param_a;
			fitness_list[i] = 0;
	}
}

float fitness_of_generations[GENERATION];
u16 param_of_generations[GENERATION];
u16 find_fitness_of_population(void)
{
	u16 para_i,i;
	float fitness = 0;
	fitness = fitness_list[0];
	if(result_to_max_or_min == 1)
	{
		for(i=1;i<POPULATION;i++)
		{
			if(fitness_list[i]>fitness)
			{
				fitness = fitness_list[i];
				para_i = i;                   
			}
		}
		return para_i;
	}
	else
	{
		for(i=1;i<POPULATION;i++)
		{
			if(fitness_list[i]<fitness)
			{
				fitness = fitness_list[i];
				para_i = i;                   
			}
		}
		return para_i;
	}
}

void add_result_to_generations_list(u16 generation_i)
{
	u16 para_i;
	para_i=find_fitness_of_population();
	if(generation_i<GENERATION)
	{
		fitness_of_generations[generation_i]=fitness_list[para_i];
		param_of_generations[generation_i]=param_list[para_i];
	}
}

mut_status mut_one(u16 *param)
{
	double p_m_buff;
	u8 point;
	u8 point_num;
	int16_t param_buff;
	int i;
	p_m_buff=(RNG_Get_RandomRange(0,1000)/1000);
	if(p_m_buff<P_MUT)
	{
		point=(int)(RNG_Get_RandomRange(1,6)+0.5);
		point_num=(int)(RNG_Get_RandomRange(0,20));
		point_num=point_num-10;
		for(i=1;i<point;i++)point_num=point_num*10;
		param_buff=*param+point_num;
		if((param_buff>param_max)||(param_buff<param_min))
			return mut_failed;
		else
		{
			*param=param_buff;
			return mut_success;
		}
	}
	else
		return mut_success;
}

void cross_once(u16 *a,u16 *b, u16 num1,u16 num2)
{
	u16 buff_a[6],buff_b[6];
	double p_c;
	p_c=(RNG_Get_RandomRange(0,1000)/1000);
	if(p_c<P_CROSS)
	{
		buff_a[5]=*a/100000;
		buff_a[4]=(*a/10000)%10;
		buff_a[3]=(*a/1000)%10;
		buff_a[2]=(*a/100)%10;
		buff_a[1]=(*a/10)%10;
		buff_a[0]=*a%10;
		buff_b[5]=*b/100000;
		buff_b[4]=(*b/10000)%10;
		buff_b[3]=(*b/1000)%10;
		buff_b[2]=(*b/100)%10;
		buff_b[1]=(*b/10)%10;
		buff_b[0]=*b%10;
		if(result_to_max_or_min == 1)
		{
			if(fitness_list[num1]>fitness_list[num2])
			{
				buff_b[5]=buff_a[5];
				buff_b[4]=buff_a[4];
				buff_b[3]=buff_a[3];
				buff_b[2]=buff_a[2];
				//buff_b[1]=buff_a[1];
				*b = buff_b[5]*100000+buff_b[4]*10000+buff_b[3]*1000
							+buff_b[2]*100+buff_b[1]*10+buff_b[0];
			}
			else if(fitness_list[num1]<fitness_list[num2])
			{			
				buff_a[5]=buff_b[5];
				buff_a[4]=buff_b[4];
				buff_a[3]=buff_b[3];
				buff_a[2]=buff_b[2];
				//buff_a[1]=buff_b[1];
				*a = buff_a[5]*100000+buff_a[4]*10000+buff_a[3]*1000
							+buff_a[2]*100+buff_a[1]*10+buff_a[0];
			}
			else ;
		}
		else
		{
			if(fitness_list[num1]<fitness_list[num2])
			{
				buff_b[5]=buff_a[5];
				buff_b[4]=buff_a[4];
				buff_b[3]=buff_a[3];
				buff_b[2]=buff_a[2];
				//buff_b[1]=buff_a[1];
				*b = buff_b[5]*100000+buff_b[4]*10000+buff_b[3]*1000
							+buff_b[2]*100+buff_b[1]*10+buff_b[0];
			}
			else if(fitness_list[num1]>fitness_list[num2])
			{			
				buff_a[5]=buff_b[5];
				buff_a[4]=buff_b[4];
				buff_a[3]=buff_b[3];
				buff_a[2]=buff_b[2];
				//buff_a[1]=buff_b[1];
				*a = buff_a[5]*100000+buff_a[4]*10000+buff_a[3]*1000
							+buff_a[2]*100+buff_a[1]*10+buff_a[0];
			}
			else;
		}
	}
}

extern float speed_error_list[];
extern float ctrl_output_list[];
double ga_abs(double a)
{
	if(a>0)return a;
	else return -a;
}
float caculate_fitness_once(void)
{
	float fitness;
	u16 i;
	for(i=0;i<simulate_cnt;i++)
	{
		fitness = fitness + weight2*(ctrl_output_list[i]*ctrl_output_list[i]);
		fitness = fitness + weight1*ga_abs(speed_error_list[i]);
		if(speed_error_list[i]<0)fitness = fitness + weight3*ga_abs(speed_error_list[i]);
	}
	return fitness;
}

//计算用于产生下一代的适应度
void get_fitness(void)
{
	u16 i;
	double fitness_line;
	double max,min;
	max=min = fitness_list[0];
	for(i=0;i<POPULATION;i++)
	{
		fitness_line+=fitness_list[i];
		if(fitness_list[i]>max)max=fitness_list[i];
		if(fitness_list[i]<min)min=fitness_list[i];
	}
	fitness_line = fitness_line/POPULATION;
	for(i=0;i<POPULATION;i++)
	{
		if(result_to_max_or_min == 1)
			{if(fitness_list[i]<fitness_line)fitness_list[i]=min;}
		else
			{if(fitness_list[i]>fitness_line)fitness_list[i]=max;}
	}
}
















