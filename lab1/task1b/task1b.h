// Peter Zhong
// 1936889
// 06/30/2021
// This is the header file for Lab 1 task 1 part b. It includes macros to memory
// addresses for GPIO port J and port N.

#ifndef __TASK1B_H__
#define __TASK1B_H__

#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))

#define RCGCGPIO_N_EN 0x00001000
#define RCGCGPIO_J_EN 0x00000100

#define GPIODIR_N (*((volatile uint32_t *)0x40064400))
#define GPIODIR_J (*((volatile uint32_t *)0x40060400))

#define GPIODEN_N (*((volatile uint32_t *)0x4006451C))
#define GPIODEN_J (*((volatile uint32_t *)0x4006051C))

#define GPIODATA_N (*((volatile uint32_t *)0x400643FC))
#define GPIODATA_J (*((volatile uint32_t *)0x400603FC))

#define GPIOPUR_J (*((volatile uint32_t *)0x40060510))

#endif //__TASK1B_H__