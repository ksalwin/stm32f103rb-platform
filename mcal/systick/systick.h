#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f1xx.h"

typedef enum {
	SYSTICK_STATE_UNINIT	= 0,
	SYSTICK_STATE_INIT,
	SYSTICK_STATE_RUNNING,
	SYSTICK_STATE_STOPPED
} systick_state_t;


void 			systick_init(void);
systick_state_t	systick_get_state(void);

// ISR; name forced by CMSIS startup script
void SysTick_Handler(void);

#endif
