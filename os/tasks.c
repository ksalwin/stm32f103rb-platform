#include "tasks.h"
#include "stm32f1xx.h"
#include "gpio.h"

void task_blink_led(void) {
	static uint32_t i = 0;

	if(i%2 == 0)
		gpio_pin_set(GPIOA, 5);
	else
		gpio_pin_reset(GPIOA, 5);

	i++;
}


task_t task_list[] = {
	{task_blink_led,	1000,	0}
};

const uint32_t task_count = sizeof(task_list) / sizeof(task_t);
