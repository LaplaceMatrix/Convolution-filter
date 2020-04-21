#include "eigamath.h"
#include "eigamain.h"
#include "ctrl.h"
#include "display.h"

extern ga_t ga;
u16 generation_cnt=0;
void ga_main(void)
{
	u16 i;
	if(generation_cnt<1)
	{
		for(i=0;i<POPULATION;i++)
		{
			simulation_once(i);
			display_param(refresh);
			display_fitness();
		}
		generation_cnt++;
	}
	display_param(none);
	display_fitness();
}
	





























