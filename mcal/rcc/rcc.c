#include "rcc.h"
#include "rcc_cfg.h"
#include "stm32f1xx.h"

static void configure_clock_HCLK(void);
static void configure_clock_pll(void);
static void configure_clock_source(void);

static void enable_clock_gpioa(void);

void rcc_init(void) {
	configure_clock_source();

	configure_clock_pll();

	configure_clock_HCLK();

	// Enable peripheral clocks
	enable_clock_gpioa();
}

/*** Static functions ***/
static void configure_clock_HCLK(void) {
	CLEAR_BIT(RCC->CFGR, RCC_CFGR_HPRE);
	// HCLK = 32 MHz
}
static void configure_clock_pll(void) {
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
}
static void configure_clock_source(void) {
	// Enable HSI as clock source
	SET_BIT(RCC->CR, RCC_CR_HSION);
	// HSI = 8 MHz

	// Wait for stable HSI
	while(READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0);
}
static void enable_clock_gpioa(void) {
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
}
