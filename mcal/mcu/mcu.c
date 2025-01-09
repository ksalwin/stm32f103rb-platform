#include "mcu.h"
#include "mcu_cfg.h"
#include "stm32f1xx.h"

void mcu_clock_init(void) {
	/***** Clock source *****/
	// Enable HSI as clock source
	SET_BIT(RCC->CR, RCC_CR_HSION);
	// HSI = 8 MHz

	// Wait for stable HSI
	while(READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0);

	/***** PLL cfg *****/
	// PLL entry clock source: HSI/2
	CLEAR_BIT(RCC->CFGR, RCC_CFGR_PLLSRC);

	// PLL multiplication factor
	SET_BIT(RCC->CFGR, RCC_CFGR_PLLMULL8);
	// SYSCLK is HSI(4) * PLL(8) = 32 MHz

	// System clock switch: PLL 
	SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);

	// Enable PLL
	SET_BIT(RCC->CR, RCC_CR_PLLON);

	// Wait for PLL ready
	while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0);

	/***** HCLK/AHB prescaler  *****/
	// Prescaler 1
	CLEAR_BIT(RCC->CFGR, RCC_CFGR_HPRE);
	// HCLK = 32 MHz
}
