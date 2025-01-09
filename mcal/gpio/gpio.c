#include "gpio.h"
#include "gpio_cfg.h"
#include "stm32f1xx.h"

#define GPIO_PINS_IN_CRL_REG	(8u)

void gpio_init(void) {
	uint8_t i;
	//uint8_t pin;

	// Initialize all pins according to the gpio_cfg
	for(i = 0; i < gpio_cfg_count; i++)
	{
		//pin = gpio_cfg[i].pin;

		if(gpio_cfg[i].pin < GPIO_PINS_IN_CRL_REG)
		{
			// Clear mode and cnf of a pin
			gpio_cfg[i].GPIOx->CRL &= ~(0x0000000f << (gpio_cfg[i].pin * 4));

			// Set cnf 
			SET_BIT(
				gpio_cfg[i].GPIOx->CRL,
				(uint32_t) gpio_cfg[i].cnf << (gpio_cfg[i].pin * 4)
			);

			// Set mode
			SET_BIT(
				gpio_cfg[i].GPIOx->CRL,
				(uint32_t) gpio_cfg[i].mode << (gpio_cfg[i].pin * 4)
			);

    		//GPIOA->CRL &= ~(GPIO_CRL_CNF5);
			//GPIOA->CRL |= GPIO_CRL_MODE5_1;
		    //GPIOA->CRL &= ~GPIO_CRL_MODE5_0;
		}
	}
}
