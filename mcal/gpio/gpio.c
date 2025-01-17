#include "gpio.h"
#include "gpio_cfg.h"

#define GPIO_PINS_IN_CRL_REG	(8u)

void gpio_init(void) {
	uint8_t i;

	// Initialize all pins according to the gpio_cfg
	for(i = 0; i < gpio_cfg_count; i++)
	{
		uint8_t 		pin_offset;
		__IO uint32_t	*cfg_reg;

		// Pins 0-7  are configured in CRL
		// Pins 8-15 are configured in CRH
		if(gpio_cfg[i].pin < GPIO_PINS_IN_CRL_REG) {
			cfg_reg		= &gpio_cfg[i].GPIOx->CRL;
			pin_offset	= gpio_cfg[i].pin * 4;
		} else {
			cfg_reg		= &gpio_cfg[i].GPIOx->CRH;
			pin_offset	= (gpio_cfg[i].pin - 8) * 4;
		}
			
		// Clear mode and cnf of a pin
		CLEAR_BIT(*cfg_reg, 0x0000000f << pin_offset);

		// Set cnf 
		SET_BIT(*cfg_reg, (uint32_t) gpio_cfg[i].cnf  << pin_offset);

		// Set mode
		SET_BIT(*cfg_reg, (uint32_t) gpio_cfg[i].mode << pin_offset);
	}
}
void gpio_pin_reset(GPIO_TypeDef *port, uint8_t pin) {
	port->BSRR = 1 << pin;
}
void gpio_pin_set(GPIO_TypeDef *port, uint8_t pin) {
	port->BRR = 1 << pin;
}
