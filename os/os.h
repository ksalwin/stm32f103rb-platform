#ifndef OS_H
#define OS_H

#include <stdint.h>

void os_start(void);

uint32_t os_get_tick_cntr(void);
void os_tick(void);

#endif
