# stm32f103rb-platform
Platform project for stm32f103rb.

Compilation:
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program build/stm32f103rb_platform.elf verify reset exit"
