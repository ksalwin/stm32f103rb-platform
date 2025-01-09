#include "systick.h"
#include "os.h"


static systick_state_t systick_state = SYSTICK_STATE_UNINIT;


void systick_init(void) {
	systick_state = SYSTICK_STATE_UNINIT;


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

	systick_state = SYSTICK_STATE_INIT;


	// Enable SysTick
	// - VAL loaded with RELOAD value and counts down
	SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
	systick_state = SYSTICK_STATE_RUNNING;
}

systick_state_t systick_get_state(void) {
	return systick_state;
}

void SysTick_Handler(void) {
	os_tick();
}
