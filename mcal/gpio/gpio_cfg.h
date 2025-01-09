#ifndef GPIO_CFG_H
#define GPIO_CFG_H

#include <stdio.h>
#include "stm32f1xx.h"

typedef struct {
	GPIO_TypeDef *	GPIOx;
	uint8_t			pin;
	uint8_t			mode;
	uint8_t			cnf;
} gpio_cfg_t;

extern const gpio_cfg_t	gpio_cfg[];
extern const uint8_t	gpio_cfg_count;

#endif
