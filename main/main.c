#include "gpio.h"
#include "os.h"
#include "rcc.h"


int main(void) {
	rcc_init();

	gpio_init();

	os_start();

	while(1);
}
