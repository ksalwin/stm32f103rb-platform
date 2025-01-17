#include "os.h"
#include "scheduler.h"

static uint32_t	os_tick_cntr;

void os_start(void) {
	uint32_t os_tick_cntr_prev	= 0u;

	scheduler_start();

	while(1) {
		if(os_tick_cntr_prev != os_tick_cntr) {
			scheduler();
			os_tick_cntr_prev = os_tick_cntr;
		}
	}
}

uint32_t os_get_tick(void) {
	return os_tick_cntr;
}

void os_tick(void) {
	os_tick_cntr++;
}
