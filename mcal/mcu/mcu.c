#include "mcu.h"
#include "mcu_cfg.h"
#include "stm32f1xx.h"
#include "os.h"

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

void mcu_systick_init(void) {
	// SysTick clock source: AHB/8 = 32/4 MHz = 4 MHz
	CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);

	// Clear current value register
	WRITE_REG(SysTick->VAL, 0);

	// Reload value: 4 MHz / 1000 - 1 = 4000 Hz - 1 := 1 ms
	WRITE_REG(SysTick->LOAD, 4000 - 1);



	// Set SysTick IRQ priority: lowest
	NVIC_SetPriority(SysTick_IRQn, 15);
	//nvic_set_irq_priority_systick(NVIC_IRQ_PRIORITY_LOWEST);

	// SysTick exception request (irq): en
	SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);



	// Enable SysTick
	// - VAL loaded with RELOAD value and counts down
	SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
}

void SysTick_Handler(void) {
	os_tick();
}
