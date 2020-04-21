#ifndef __EIGAMATH_H
#define __EIGAMATH_H	
#include "sys.h"


typedef struct{
	u8 population;
	u8 generation;
	float p_cross;
	float p_mut;
	float eliminate;
}ga_t;


typedef enum
{
	param_switch_kp=0,
	param_switch_ki=1,
	param_switch_kd=2
}param_switch;

typedef enum
{
	mut_success=1,
	mut_failed=0
}mut_status;





#define POPULATION			50
#define GENERATION			50
#define P_CROSS					0.5
#define P_MUT						0.01
#define ELIMAINATE			1
	 
#define param_sitch			param_switch_kp
#define param_a					100
#define fitness_a				1000

#define param_max				2*param_a
#define param_min				0*param_a

#define result_to_max_or_min  0//1为去最大值，0位最小值
	 
#define weight1					0.999   //误差权重
#define weight2					0.01		//控制输出权重
#define weight3					10			//超调权重











void ga_param_init(void);
void list_init(void);
void add_result_to_generations_list(u16 generation_i);
mut_status mut_one(u16 *param);
void cross_once(u16 *a,u16 *b, u16 num1,u16 num2);
double ga_abs(double a);
float caculate_fitness_once(void);
void get_fitness(void);










#endif





















