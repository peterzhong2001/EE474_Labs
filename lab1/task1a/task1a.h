// Peter Zhong
// 1936889
// 06/30/2021
// This is the header file for Lab 1 task 1 part a. It includes macros to memory
// addresses for GPIO port F and port N.

#ifndef __TASK1A_H__
#define __TASK1A_H__

#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))
#define RCGCGPIO_F_EN 0x00000020
#define RCGCGPIO_N_EN 0x00001000

#define GPIODIR_F (*((volatile uint32_t *)0x4005D400))
#define GPIODIR_N (*((volatile uint32_t *)0x40064400))

#define GPIODEN_F (*((volatile uint32_t *)0x4005D51C))
#define GPIODEN_N (*((volatile uint32_t *)0x4006451C))

#define GPIODATA_F (*((volatile uint32_t *)0x4005D3FC))
#define GPIODATA_N (*((volatile uint32_t *)0x400643FC))

// fixed interval function for the flashing lights
void interval();

#endif //__TASK1A_H__