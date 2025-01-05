#include "mcu.h"
#include "mcu_cfg.h"
#include "stm32f1xx.h"

void mcu_clock_init(void) {
	RCC->CR = RCC_CR_HSION;
}

void mcu_systick_init(void) {
}
