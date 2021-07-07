// Peter Zhong
// 1936889
// 06/30/2021
// This is the header file for Lab 1 task 1 part a. It includes macros to memory
// addresses for GPIO port E and definitions of helper methods for task 2.

#ifndef __TASK1B_H__
#define __TASK1B_H__

#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))
#define RCGCGPIO_E_EN 0x00000010
#define GPIOAMSEL_E (*((volatile uint32_t *)0x4005C528))
#define GPIOAFSEL_E (*((volatile uint32_t *)0x4005C420))
#define GPIODIR_E (*((volatile uint32_t *)0x4005C400))
#define GPIODEN_E (*((volatile uint32_t *)0x4005C51C))
#define GPIODATA_E (*((volatile uint32_t *)0x4005C3FC))

#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))
#define RCGCTIMER_0_EN 0x01
#define GPTMCTL_0 (*((volatile uint32_t *)0x4003000C))
#define GPTMCFG_0 (*((volatile uint32_t *)0x40030000))
#define CFG_32BIT 0x00
#define GPTMTAMR_0 (*((volatile uint32_t *)0x40030004))
#define TAMR_PERIODIC 0x0002
#define TAMR_COUNT_DOWN 0x0010
#define GPTMTAILR_0 (*((volatile uint32_t *)0x40030028))
#define FREQ_1HZ 0xF42400 // 16M in hex
#define GPTMRIS_0 (*((volatile uint32_t *)0x4003001C))
#define GPTMICR_0 (*((volatile uint32_t *)0x40030024))

// Helper functions for initializing timer
void TimerInit();
// Helper function for starting timer
void StartTimer();
// Helper function for stopping and clearing timer
void StopAndClearTimer();

// Helper functions for initializing Sys Button at PE1
void SysInit();
// Helper functions for initializing Ped Button at PE0
void PedInit();

// Takes the switch mask as parameter and return the state of that switch.
// This function only returns 1 when the switch has been pressed for at least
// 2 seconds.
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