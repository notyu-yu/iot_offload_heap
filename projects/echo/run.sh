echo "=================================================="
echo "activate openocd"
trap "kill 0" EXIT
openocd -f interface/stlink-v2.cfg -f target/stm32l4x.cfg > /dev/null 2>/dev/null &
echo "=================================================="
echo "make program"
make
gcc -g3 -o pc_echo pc_echo.c > /dev/null
echo "=================================================="
echo "burn it"
arm-none-eabi-gdb -x ./gdbrc.txt ./Debug/mcu_echo.elf
