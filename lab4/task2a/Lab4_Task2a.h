// Peter Zhong
// 1936889
// 07/06/2021
// This is the header file for Lab 2 task 1 part b. It includes macros to memory
// addresses for GPIO port E and definitions of helper methods for task 2.

#ifndef __TASK1B_H__
#define __TASK1B_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// macros for accessing GPIO ports
#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))
#define RCGCGPIO_E_EN 0x00000010

#define GPIOAMSEL_E (*((volatile uint32_t *)0x4005C528))
#define GPIOAFSEL_E (*((volatile uint32_t *)0x4005C420))
#define GPIODIR_E (*((volatile uint32_t *)0x4005C400))
#define GPIODEN_E (*((volatile uint32_t *)0x4005C51C))
#define GPIODATA_E (*((volatile uint32_t *)0x4005C3FC))

// macros for accessing Timer 0A and Timer 1A
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604))
#define RCGCTIMER_0_EN 0x01
#define RCGCTIMER_1_EN 0x02

#define GPTMCTL_0 (*((volatile uint32_t *)0x4003000C))
#define GPTMCTL_1 (*((volatile uint32_t *)0x4003100C))

#define GPTMCFG_0 (*((volatile uint32_t *)0x40030000))
#define GPTMCFG_1 (*((volatile uint32_t *)0x40031000))
#define CFG_32BIT 0x0

#define GPTMTAMR_0 (*((volatile uint32_t *)0x40030004))
#define GPTMTAMR_1 (*((volatile uint32_t *)0x40031004))
#define TAMR_ONESHOT 0x0001
#define TAMR_COUNT_DOWN 0x0000

#define GPTMTAILR_0 (*((volatile uint32_t *)0x40030028))
#define GPTMTAILR_1 (*((volatile uint32_t *)0x40031028))
#define FREQ_2S 0x7270E00 // 120M in hex
#define FREQ_5S 0x11E1A300 // 300M in hex

#define GPTMRIS_0 (*((volatile uint32_t *)0x4003001C))
#define GPTMRIS_1 (*((volatile uint32_t *)0x4003101C))

#define GPTMICR_0 (*((volatile uint32_t *)0x40030024))
#define GPTMICR_1 (*((volatile uint32_t *)0x40031024))

#define GPTMCC_0 (*((volatile uint32_t *)0x40030FC8))
#define GPTMCC_1 (*((volatile uint32_t *)0x40031FC8))

// Helper function for resetting timer
void ResetTimer0();
void ResetTimer1();

// Helper function for starting timer
void StartTimer0();
void StartTimer1();

// Helper function for clearing flags
void ClearFlagTimer0();
void ClearFlagTimer1();

// Helper function for drawing the LCD board interface
void DrawInterface();

// Takes the switch mask as parameter and return the state of that switch.
// This function only returns 1 when the switch has been pressed for at least
// 2 seconds.
unsigned char GetButton(char sw);

bool SysPressed();
bool PedPressed();

// Helper functions for red LED at PE2: initialize, on, and off
void RedOn();
void RedOff();

// Helper functions for yellow LED at PE3: initialize, on, and off
void YellowOn();
void YellowOff();

// Helper functions for green LED at PE5: initialize, on, and off
void GreenOn();
void GreenOff();

// Overall initialize function
void Init();

// 5-second interval between states. Can only be prematurely ended when either
// the system button is pressed for at least 2 seconds or the pedestrian button
// is pressed for at least 2 seconds in the go state
void Interval();

#endif //__TASK2_H__