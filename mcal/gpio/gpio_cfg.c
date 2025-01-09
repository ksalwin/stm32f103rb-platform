#include "gpio_cfg.h"

//	GPIO	Pin		MODE	CNF
const gpio_cfg_t gpio_cfg[] = {
	{GPIOA,	5,		0x00,	0x02}
};

const uint8_t gpio_cfg_count = sizeof(gpio_cfg)/sizeof(gpio_cfg_t);
