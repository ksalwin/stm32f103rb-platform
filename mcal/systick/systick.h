#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f1xx.h"

void systick_init(void);

// ISR; name forced by CMSIS startup script
void SysTick_Handler(void);

#endif
