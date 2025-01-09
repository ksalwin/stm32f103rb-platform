#include "stm32f1xx.h"
#include "gpio.h"
#include "os.h"
#include "rcc.h"

void init_LED(void);

int main(void) {
	rcc_init();

    init_LED();
	gpio_init();

	os_start();

	while(1);
}

void init_LED(void) {
    // Enable clock for GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
}
