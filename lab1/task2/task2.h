// Peter Zhong
// 1936889
// 06/30/2021
// This is the header file for Lab 1 task 1 part a. It includes macros to memory
// addresses for GPIO port E and definitions of helper methods for task 2.

#ifndef __TASK2_H__
#define __TASK2_H__

#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))

#define RCGCGPIO_E_EN 0x00000010
#define GPIOAMSEL_E (*((volatile uint32_t *)0x4005C528))
#define GPIOAFSEL_E (*((volatile uint32_t *)0x4005C420))
#define GPIODIR_E (*((volatile uint32_t *)0x4005C400))
#define GPIODEN_E (*((volatile uint32_t *)0x4005C51C))
#define GPIODATA_E (*((volatile uint32_t *)0x4005C3FC))

// Helper functions for initializing Sys Button at PE1
void SysInit();
// Helper functions for initializing Ped Button at PE0
void PedInit();

// Takes the switch mask as parameter and return the state of that switch
unsigned char GetButton(unsigned char sw);

// Helper functions for red LED at PE2: initialize, on, and off
void RedInit();
void RedOn();
void RedOff();

// Helper functions for yellow LED at PE3: initialize, on, and off
void YellowInit();
void YellowOn();
void YellowOff();

// Helper functions for green LED at PE5: initialize, on, and off
void GreenInit();
void GreenOn();
void GreenOff();

// Overall initialize function
void Init();

// Fixed delay interval function
void Interval();

#endif //__TASK2_H__