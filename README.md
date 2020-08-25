# A start up project based on STM32F429I-Discovery

## Enviroment

* Ubuntu 20.04 LTS
* gcc-arm-none-abi
* binutils-arm-none-abi
* gdb-multiarch
* gef(gdb enchanced script)
* openocd
* qemu-system-mcueclipse
* make

## Enviroment Set Up

* gnu tool-chains & gnu debugger

```
$ sudo apt-get install gcc-arm-none-abi binutils-arm-none-abi gdb-multiarch
```

* qemu-arm-mcueclipse

qemu-arm-mcueclipse is a branch of offical QEMU, and it is under [the xPack Project](https://xpack.github.io/). It supports emulations of stm32fxx to stmf4xx SoC and relevent boards, including Nucleo and Discovery. The functions of emulating stm32f4 series is not supported fully than stm32f0 ones, but it is still enough to test basically program such as blinky led or rtos task switch demo. The more informations are in [the xPack QEMU Arm](https://xpack.github.io/qemu-arm/).
* [xPack Package Manager install](https://xpack.github.io/xpm/install/)
* [xPack QEMU Arm set up](https://xpack.github.io/qemu-arm/install/)

## Resources
* STM32CubeF4

## References
Repos
  * [embedded2015/freertos-basic](https://github.com/embedded2015/freertos-basic)
  * [lochsh/gpio-toggle-exercise](https://github.com/lochsh/gpio-toggle-exercise)
  * [jeremyherbert/stm32-templates](https://github.com/jeremyherbert/stm32-templates)
