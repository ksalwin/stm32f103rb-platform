#include "os.h"
#include "mcu.h"

void os_init(void) {
	mcu_systick_init();
}
