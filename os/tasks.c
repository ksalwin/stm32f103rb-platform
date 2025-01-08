#include "tasks.h"
#include "stm32f1xx.h"


void task_blink_led(void) {
	static uint32_t i = 0;

	if(i%2 == 0)
		GPIOA->BSRR = GPIO_BSRR_BS5; // Turn on LED (set PA5 high)
	else
		GPIOA->BSRR = GPIO_BSRR_BR5; // Turn off LED (set PA5 low)

	i++;
}


task_t task_list[] = {
	{task_blink_led,	1000,	0}
};

const uint32_t task_count = sizeof(task_list) / sizeof(task_t);
