#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_init(void);
uint8_t gpio_get_state(void);

void gpio_reset	(uint8_t pin);
void gpio_set	(uint8_t pin);

#endif
