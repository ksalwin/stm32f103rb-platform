#include "os_scheduler.h"
#include "tasks.h"
#include "systick.h"

void os_scheduler(void) {
	uint32_t i;


	for(i = 0; i < task_count; i++)
	{
		if(task_list[i].offset_ms == 0)
		{
			task_list[i].function();
			task_list[i].offset_ms = task_list[i].period_ms;
		}

		task_list[i].offset_ms--;
	}
}

void os_scheduler_start(void) {
	systick_init();
}
