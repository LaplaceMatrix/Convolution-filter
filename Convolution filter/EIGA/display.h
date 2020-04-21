#ifndef __DISPLAY_H
#define __DISPLAY_H		
#include "sys.h"


typedef enum 
{
	refresh = 1,
	none = 0
}refresh_commander_t;


void display_init(void);
void display_param(refresh_commander_t refresh_commander);
u16 display_signal(void);
void display_fitness(void);







#endif  
	 
	 



