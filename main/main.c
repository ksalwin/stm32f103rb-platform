#include "stm32f1xx.h"
#include "gpio.h"
#include "mcu.h"
#include "os.h"

void init_LED(void);

int main(void) {
	mcu_clock_init();

    init_LED();
	gpio_init();

	os_start();

	while(1);
}

void init_LED(void) {
    // Enable clock for GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // Configure PA5 as output push-pull, max speed 2 MHz
    //GPIOA->CRL &= ~(GPIO_CRL_CNF5); // Clear CNF5 (set to 00: General purpose output push-pull)
    //GPIOA->CRL |= GPIO_CRL_MODE5_1; // Set MODE5 to 10 (output mode, max speed 2 MHz)
    //GPIOA->CRL &= ~GPIO_CRL_MODE5_0;
}
