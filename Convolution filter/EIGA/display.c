#include "display.h"
#include "lcd.h"
#include "key.h"
#include "eigamath.h"


#define width  270
#define hight  478


extern u16 param_list[];
extern float fitness_list[];

void display_init(void)
{
	u8 i = 0;
  LCD_Init();                     		//≥ı ºªØLCD
	LCD_Clear(BLACK);
	POINT_COLOR=YELLOW;	  	 
	LCD_DrawRectangle(0,0,width,hight/2);
	LCD_ShowString(0,hight/2,16*13,16,16,"best_fitness:");
	LCD_ShowString(0,hight/2+16,16*10,16,16,"fitest_kp:");
	for(i=0;i<13;i++)
	{
		LCD_ShowxNum(0,hight/2+(i+2)*16,i+1,2,16,1);
		LCD_ShowxNum(width/2,hight/2+(i+2)*16,i+14,2,16,1);
	}
	for(i=0;i<13;i++)
	{
		POINT_COLOR=GREEN;
		if(i<POPULATION)LCD_ShowxNum(0+2*8,hight/2+(i+2)*16,param_list[i],7,16,1);
		if(i+13<POPULATION)LCD_ShowxNum(width/2+2*8,hight/2+(i+2)*16,param_list[i+13],7,16,1);
		POINT_COLOR=BLUE;
		if(i<POPULATION)LCD_ShowxNum(0+9*8,hight/2+(i+2)*16,fitness_list[i],7,16,1);
		if(i+13<POPULATION)LCD_ShowxNum(width/2+9*8,hight/2+(i+2)*16,fitness_list[i+13],7,16,1);
	}
}


#define page_num 3
int last_page = 0;
int page = 0;
void display_param(refresh_commander_t refresh_commander)
{
	u8 key,i;
	key=KEY_Scan(0);
	if(key==KEY0_PRES)page++;
	else if(key==KEY2_PRES)page--;
	if(page>page_num)page=0;
	if(page<0)page=page_num;
	
	if(page!=last_page||refresh_commander==refresh)
	{
		switch(page)
		{
			case 0:
			{
				LCD_Fill(0,hight/2+1,width,hight,BLACK);
				POINT_COLOR=YELLOW;	
				LCD_ShowString(0,hight/2,16*13,16,16,"best_fitness:");
				LCD_ShowString(0,hight/2+16,16*10,16,16,"fitest_kp:");
				for(i=0;i<13;i++)
				{
					LCD_ShowxNum(0,hight/2+(i+2)*16,i+1,2,16,1);
					LCD_ShowxNum(width/2,hight/2+(i+2)*16,i+14,2,16,1);
				}	
				for (i=0;i<13;i++)
				{
					POINT_COLOR=GREEN;
					if(i<POPULATION)LCD_ShowxNum(0+2*8,hight/2+(i+2)*16,param_list[i],7,16,1);
					if(i+13<POPULATION)LCD_ShowxNum(width/2+2*8,hight/2+(i+2)*16,param_list[i+13],7,16,1);
					POINT_COLOR=BLUE;
					if(i<POPULATION)LCD_ShowxNum(0+9*8,hight/2+(i+2)*16,fitness_list[i],7,16,1);
					if(i+13<POPULATION)LCD_ShowxNum(width/2+9*8,hight/2+(i+2)*16,fitness_list[i+13],7,16,1);
				}
			}
			break;
			case 1:
			{
				LCD_Fill(0,hight/2+1,width,hight,BLACK);
				POINT_COLOR=YELLOW;	
				LCD_ShowString(0,hight/2,16*13,16,16,"best_fitness:");
				LCD_ShowString(0,hight/2+16,16*10,16,16,"fitest_kp:");
				for(i=0;i<13;i++)
				{
					LCD_ShowxNum(0,hight/2+(i+2)*16,i+27,2,16,1);
					LCD_ShowxNum(width/2,hight/2+(i+2)*16,i+40,2,16,1);
				}
				for (i=0;i<13;i++)
				{
					POINT_COLOR=GREEN;	
					if(i+26<POPULATION)LCD_ShowxNum(0+2*8,hight/2+(i+2)*16,param_list[i+26],7,16,1);
					if(i+39<POPULATION)LCD_ShowxNum(width/2+2*8,hight/2+(i+2)*16,param_list[i+39],7,16,1);
					POINT_COLOR=BLUE;
					if(i+26<POPULATION)LCD_ShowxNum(0+9*8,hight/2+(i+2)*16,fitness_list[i+26],7,16,1);
					if(i+39<POPULATION)LCD_ShowxNum(width/2+9*8,hight/2+(i+2)*16,fitness_list[i+39],7,16,1);
				}
			}
			break;
			case 2:
			{
				LCD_Fill(0,hight/2+1,width,hight,BLACK);
				POINT_COLOR=YELLOW;	
				LCD_ShowString(0,hight/2,16*13,16,16,"best_fitness:");
				LCD_ShowString(0,hight/2+16,16*10,16,16,"fitest_kp:");
				for(i=0;i<13;i++)
				{
					LCD_ShowxNum(0,hight/2+(i+2)*16,i+53,2,16,1);
					LCD_ShowxNum(width/2,hight/2+(i+2)*16,i+65,2,16,1);
				}	
				for (i=0;i<13;i++)
				{
					POINT_COLOR=GREEN;
					if(i+52<POPULATION)LCD_ShowxNum(0+2*8,hight/2+(i+2)*16,param_list[i+52],7,16,1);
					if(i+64<POPULATION)LCD_ShowxNum(width/2+2*8,hight/2+(i+2)*16,param_list[i+64],7,16,1);
					POINT_COLOR=BLUE;
					if(i+52<POPULATION)LCD_ShowxNum(0+2*8,hight/2+(i+2)*16,fitness_list[i+52],7,16,1);
					if(i+64<POPULATION)LCD_ShowxNum(width/2+2*8,hight/2+(i+2)*16,fitness_list[i+64],7,16,1);
				}
			}
			break;
			case 3:
			{
				LCD_Fill(0,hight/2+1,width,hight,BLACK);
				POINT_COLOR=YELLOW;	
				LCD_ShowString(0,hight/2,16*13,16,16,"best_fitness:");
				LCD_ShowString(0,hight/2+16,16*10,16,16,"fitest_kp:");
				for(i=0;i<13;i++)
				{
					LCD_ShowxNum(0,hight/2+(i+2)*16,i+79,2,16,1);
					LCD_ShowxNum(width/2,hight/2+(i+2)*16,i+91,2,16,1);
				}
				for (i=0;i<13;i++)
				{
					POINT_COLOR=GREEN;	
					if(i+78<POPULATION)LCD_ShowxNum(0+2*8,hight/2+(i+2)*16,param_list[i+78],7,16,1);
					if(i+90<POPULATION)LCD_ShowxNum(width/2+2*8,hight/2+(i+2)*16,param_list[i+90],7,16,1);
					POINT_COLOR=BLUE;	
					if(i+78<POPULATION)LCD_ShowxNum(0+9*8,hight/2+(i+2)*16,param_list[i+78],7,16,1);
					if(i+90<POPULATION)LCD_ShowxNum(width/2+9*8,hight/2+(i+2)*16,param_list[i+90],7,16,1);
				}
			}
		}
	}
	last_page = page;
}
extern u16 fitness_of_generations[];
u16 display_signal(void)
{
	u16 i,max_i=0;
	double a_x=0,a_y=0;
	u16 fitness_max=fitness_of_generations[0];
	for(i=1;i<GENERATION;i++)
	{
		if(fitness_of_generations[i]>fitness_max)
		{
			fitness_max=fitness_of_generations[i];
			max_i=i;
		}
	}
	a_x = width/GENERATION;
	if(fitness_max!=0)a_y = ((int)(hight/2))/fitness_max;
	else a_y = ((int)(hight/2))/10;
	
	POINT_COLOR=WHITE;
	for(i=0;i<GENERATION-1;i++)
	{
		LCD_DrawLine((u16)(i*a_x+0.5),(u16)(hight/2-fitness_of_generations[i]*a_y+0.5),
						(u16)((i+1)*a_x+0.5),(u16)(hight/2-fitness_of_generations[i+1]*a_y+0.5));
	}
	return max_i;
}
extern u16 param_of_generations[];
void display_fitness(void)
{
	u16 max_i = 0;
	max_i = display_signal();
	POINT_COLOR=YELLOW;
	LCD_ShowString(0,hight/2,16*13,16,16,"best_fitness:");
	LCD_ShowString(0,hight/2+16,16*10,16,16,"fitest_kp:");
	POINT_COLOR=WHITE;
	LCD_ShowxNum(0+12*8,hight/2,fitness_of_generations[max_i],7,16,1);
	LCD_ShowxNum(0+10*8,hight/2+16,param_of_generations[max_i],7,16,1);
}

