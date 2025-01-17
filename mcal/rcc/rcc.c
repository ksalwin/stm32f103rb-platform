#include "rcc.h"
#include "rcc_cfg.h"
#include "stm32f1xx.h"

static void configure_clock_source(void);

static void enable_gpioa_clk(void);

void rcc_init(void) {
	configure_clock_source();
	
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

	enable_gpioa_clk();
}

/*** Static functions ***/

static void configure_clock_source(void) {
	// Enable HSI as clock source
	SET_BIT(RCC->CR, RCC_CR_HSION);
	// HSI = 8 MHz

	// Wait for stable HSI
	while(READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0);
}
static void enable_gpioa_clk(void) {
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
}
