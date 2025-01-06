#include "stm32f1xx.h"
#include "os.h"
#include "mcu.h"

void delay_ms(uint32_t ms);
void init_LED(void);

int main(void) {
	mcu_clock_init();
	os_init();

    init_LED();
	while(1)
	{
		__NOP();
	}

    while (1) {
        GPIOA->BSRR = GPIO_BSRR_BS5; // Turn on LED (set PA5 high)
        delay_ms(500);

        GPIOA->BSRR = GPIO_BSRR_BR5; // Turn off LED (set PA5 low)
        delay_ms(500);
    }
}

void init_LED(void) {
    // Enable clock for GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // Configure PA5 as output push-pull, max speed 2 MHz
    GPIOA->CRL &= ~(GPIO_CRL_CNF5); // Clear CNF5 (set to 00: General purpose output push-pull)
    GPIOA->CRL |= GPIO_CRL_MODE5_1; // Set MODE5 to 10 (output mode, max speed 2 MHz)
    GPIOA->CRL &= ~GPIO_CRL_MODE5_0;
}

void delay_ms(uint32_t ms) {
    // A basic delay loop. Adjust the loop count for your clock speed.
    volatile uint32_t count;
    for (uint32_t i = 0; i < ms; i++) {
        count = 1000; // Assuming a 72 MHz system clock
        while (count--) {
            __NOP(); // Prevent optimization
        }
    }
}
