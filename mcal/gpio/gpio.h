#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "stm32f1xx.h"

void gpio_init(void);
uint8_t gpio_get_state(void);

void gpio_pin_reset	(GPIO_TypeDef *port, uint8_t pin);
void gpio_pin_set	(GPIO_TypeDef *port, uint8_t pin);

#endif
