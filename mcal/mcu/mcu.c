#include "mcu.h"
#include "mcu_cfg.h"
#include "stm32f1xx.h"

void mcu_clock_init(void) {
	/***** Clock source *****/
	// Enable HSI as clock source
	SET_BIT(RCC->CR, RCC_CR_HSION);

	// Wait for stable HSI
	while(READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0);

	/***** PLL cfg *****/
	// PLL entry clock source: HSI/2
	CLEAR_BIT(RCC->CFGR, RCC_CFGR_PLLSRC);

	// PLL multiplication factor
	SET_BIT(RCC->CFGR, RCC_CFGR_PLLMULL4);

	// System clock switch: PLL 
	SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);

	// Enable PLL
	SET_BIT(RCC->CR, RCC_CR_PLLON);

	// Wait for PLL ready
	while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0);
}

void mcu_systick_init(void) {
}
